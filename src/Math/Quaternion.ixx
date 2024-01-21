module;

#include <cmath>
export module NYANMath:Quaternion;

import :Util;
import :Constants;
import :Vector;
import :Matrix;

export namespace Math
{

	template<ScalarT Scalar>
	class Quaternion {
	public:
		using value_type = Scalar;
		constexpr Quaternion() noexcept :
			m_real(),
			m_imaginary()
		{

		}
		constexpr explicit Quaternion(Scalar real, Vec<Scalar, 3> imaginary) noexcept :
			m_real(real),
			m_imaginary(imaginary)
		{

		}
		constexpr explicit Quaternion(Scalar real, Scalar imaginary1, Scalar imaginary2, Scalar imaginary3) noexcept :
			m_real(real),
			m_imaginary({ imaginary1 , imaginary2 , imaginary3 })
		{

		}
		template<size_t X, size_t Y, bool column_major>
		constexpr explicit Quaternion(const Mat<Scalar, X, Y, column_major>& mat) noexcept
		{
			static_assert(X == 3 || X == 4);
			static_assert(Y == 3 || Y == 4);
			Scalar t{ 0 };
			if (mat(2, 2) < 0) {
				if (mat(0, 0) > mat(1, 1)) {
					t = 1 + mat(0, 0) - mat(1, 1) - mat(2, 2);
					m_real = t;
					m_imaginary[0] = mat(0, 1) + mat(1, 0);
					m_imaginary[1] = mat(0, 2) + mat(2, 0);
					m_imaginary[2] = mat(2, 1) - mat(1, 2);
				}
				else {
					t = 1 - mat(0, 0) + mat(1, 1) - mat(2, 2);
					m_real = mat(0, 1) + mat(1, 0);
					m_imaginary[0] = t;
					m_imaginary[1] = mat(2, 1) + mat(1, 2);
					m_imaginary[2] = mat(0, 2) - mat(2, 0);
				}
			}
			else {
				if (mat(0, 0) < -mat(1, 1)) {
					t = 1 - mat(0, 0) - mat(1, 1) + mat(2, 2);
					m_real = mat(0, 2) + mat(2, 0);
					m_imaginary[0] = mat(2, 1) + mat(1, 2);
					m_imaginary[1] = t;
					m_imaginary[2] = mat(1, 0) - mat(0, 1);
				}
				else {
					t = 1 + mat(0, 0) + mat(1, 1) + mat(2, 2);
					m_real = mat(2, 1) - mat(1, 2);
					m_imaginary[0] = mat(0, 2) + mat(2, 0);
					m_imaginary[1] = mat(1, 0) - mat(0, 1);
					m_imaginary[2] = t;
				}
			}
			*this *= 0.5 / sqrt(t);
		}
		constexpr explicit Quaternion(const Vec<Scalar, 4>& vec) noexcept
		{
			m_real = vec[0];
			m_imaginary[0] = vec[1];
			m_imaginary[1] = vec[2];
			m_imaginary[2] = vec[3];
		}
		constexpr Scalar get_real() const noexcept {
			return m_real;
		}
		constexpr const Vec<Scalar, 3>& get_imaginary() const noexcept
		{
			return m_imaginary;
		}
		// Expects Angles in Degrees
		// Reason being: this is expected to be used as a human interface and not intended for internal use, this is what quaternions are for
		// Yaw (Z) -> Pitch (Y) -> Roll (X)
		explicit Quaternion(Scalar x_roll, Scalar y_pitch, Scalar z_yaw) noexcept
		{
			//Source https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
			double cy = cos(z_yaw * 0.5 * deg_to_rad);
			double sy = sin(z_yaw * 0.5 * deg_to_rad);
			double cp = cos(y_pitch * 0.5 * deg_to_rad);
			double sp = sin(y_pitch * 0.5 * deg_to_rad);
			double cr = cos(x_roll * 0.5 * deg_to_rad);
			double sr = sin(x_roll * 0.5 * deg_to_rad);
			m_real = Scalar(cy * cp * cr + sy * sp * sr);
			m_imaginary[0] = Scalar(cy * cp * sr - sy * sp * cr);
			m_imaginary[1] = Scalar(sy * cp * sr + cy * sp * cr);
			m_imaginary[2] = Scalar(sy * cp * cr - cy * sp * sr);
		}
		// Expects Angles in Degrees
		// Reason being: this is expected to be used as a human interface and not intended for internal use, this is what quaternions are for
		// Yaw (Z) -> Pitch (Y) -> Roll (X)
		explicit Quaternion(Vec<Scalar, 3> euler_angles) noexcept {
			//Source https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
			double cy = cos(euler_angles[2] * 0.5 * deg_to_rad);
			double sy = sin(euler_angles[2] * 0.5 * deg_to_rad);
			double cp = cos(euler_angles[1] * 0.5 * deg_to_rad);
			double sp = sin(euler_angles[1] * 0.5 * deg_to_rad);
			double cr = cos(euler_angles[0] * 0.5 * deg_to_rad);
			double sr = sin(euler_angles[0] * 0.5 * deg_to_rad);
			m_real = Scalar(cy * cp * cr + sy * sp * sr);
			m_imaginary[0] = Scalar(cy * cp * sr - sy * sp * cr);
			m_imaginary[1] = Scalar(sy * cp * sr + cy * sp * cr);
			m_imaginary[2] = Scalar(sy * cp * cr - cy * sp * sr);
		}
		constexpr friend inline bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			return (lhs.m_real == rhs.m_real) && (lhs.m_imaginary == rhs.m_imaginary);
		}
		constexpr friend inline bool close(const Quaternion& lhs, const Quaternion& rhs, const Scalar& eps = static_cast<Scalar>(1e-5)) noexcept
		{
			return close(lhs.m_real, rhs.m_real, eps) && close(lhs.m_imaginary, rhs.m_imaginary, eps);
		}
		constexpr friend inline Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)  noexcept
		{
			Quaternion result;
			result.m_real = lhs.m_real + rhs.m_real;
			result.m_imaginary = lhs.m_imaginary + rhs.m_imaginary;
			return result;
		}
		constexpr friend inline Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs)  noexcept
		{
			return { lhs.m_real - rhs.m_real ,
				lhs.m_imaginary - rhs.m_imaginary };
		}
		constexpr friend inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			return { lhs.m_real * rhs.m_real - lhs.m_imaginary.dot(rhs.m_imaginary) ,
					lhs.m_real* rhs.m_imaginary + rhs.m_real * lhs.m_imaginary + cross(lhs.m_imaginary, rhs.m_imaginary) };
		}
		constexpr friend inline Quaternion operator*(const Quaternion& lhs, const Scalar& rhs) noexcept
		{
			return { lhs.m_real * rhs,
					lhs.m_imaginary * rhs};
		}
		//constexpr inline std::string convert_to_string() {
		//	std::string result("(");
		//	result += std::to_string(m_real) + ", ";
		//	result += m_imaginary.convert_to_string() + ")";
		//	return result;
		//}
		constexpr friend inline Vec<Scalar, 3> operator*(const Quaternion& lhs, const Vec<Scalar, 3>& rhs) noexcept
		{
			//Vec<Scalar, 3> result = Scalar(2) * dot(m_imaginary, rhs) * m_imaginary + (square(m_real) - dot(m_imaginary, m_imaginary)) * rhs + Scalar(2) * m_real * (cross(m_imaginary, rhs));
/*			return Scalar(2.) * lhs.m_imaginary.dot(rhs) * lhs.m_imaginary
				+ (square(lhs.m_real) - lhs.m_imaginary.dot(lhs.m_imaginary)) * rhs
				+ Scalar(2.) * lhs.m_real * lhs.m_imaginary.cross(rhs);		*/
			return rhs + ((lhs.m_imaginary.cross(rhs) * lhs.m_real) + lhs.m_imaginary.cross(lhs.m_imaginary.cross(rhs))) * 2.f;
			/* As suggested by Horn, I don't know which one is right
			Scalar xx = lhs.m_imaginary[0] * lhs.m_imaginary[0];
			Scalar xy = lhs.m_imaginary[0] * lhs.m_imaginary[1];
			Scalar xz = lhs.m_imaginary[0] * lhs.m_imaginary[2];
			Scalar yy = lhs.m_imaginary[1] * lhs.m_imaginary[1];
			Scalar yz = lhs.m_imaginary[1] * lhs.m_imaginary[2];
			Scalar zz = lhs.m_imaginary[2] * lhs.m_imaginary[2];
			Scalar ww = lhs.m_real * lhs.m_real;
			Scalar wx = lhs.m_real * lhs.m_imaginary[0];
			Scalar wy = lhs.m_real * lhs.m_imaginary[1];
			Scalar wz = lhs.m_real * lhs.m_imaginary[2];
			return Vec<Scalar, 3> ({
				rhs[0] * (ww + xx - yy - zz) + rhs[1]* Scalar(2) * (xy - wz) +rhs[2] * Scalar(2) * (wy + xz),
				rhs[0] * Scalar(2) * (xy + wz) + rhs[1] * (ww - xx + yy - zz) + rhs[2] * Scalar(2) * (yz - wx),
				rhs[0] * Scalar(2) * (xz - wy) + rhs[1] * Scalar(2) * (yz + wz) + rhs[2] * (ww - xx - yy +zz)
			});
			*/
		}
		constexpr friend inline Vec<Scalar, 4> operator*(const Quaternion& lhs, const Vec<Scalar, 4>& rhs) noexcept
		{
			//Quaternion result = lhs * Quaternion(0, Vec<Scalar, 3>({ rhs[0], rhs[1], rhs[2] })) * lhs.inverse();
			//return Vec<Scalar, 4>({result.m_imaginary[0], result.m_imaginary[1], result.m_imaginary[2], Scalar(1)});
			auto tmp = Vec<Scalar, 3>(rhs);
			tmp = Scalar(2.) * lhs.m_imaginary.dot(tmp) * lhs.m_imaginary + (square(lhs.m_real) - lhs.m_imaginary.dot(lhs.m_imaginary)) * tmp + Scalar(2.) * lhs.m_real * lhs.m_imaginary.cross(tmp);
			return Vec<Scalar, 4>({ tmp.x(), tmp.y(), tmp.z(), rhs[3] });
		}
		constexpr friend inline Quaternion operator/(const Quaternion& lhs, const Scalar& rhs) noexcept
		{
			Quaternion result;
			result.m_real = lhs.m_real / rhs;
			result.m_imaginary = lhs.m_imaginary / rhs;
			return result;
		}
		constexpr inline Quaternion& operator+=(const Quaternion& rhs) noexcept
		{
			m_real += rhs.m_real;
			m_imaginary += rhs.m_imaginary;
			return *this;
		}
		constexpr inline Quaternion& operator-=(const Quaternion& rhs) noexcept
		{
			m_real -= rhs.m_real;
			m_imaginary -= rhs.m_imaginary;
			return *this;
		}
		constexpr inline Quaternion& operator*=(const Quaternion& rhs) noexcept
		{
			m_real = m_real * rhs.m_real - dot(m_imaginary, rhs.m_imaginary);
			m_imaginary = m_real * rhs.m_imaginary + rhs.m_real * m_imaginary + cross(m_imaginary, rhs.m_imaginary);
			return *this;
		}
		constexpr inline Quaternion& operator*=(const Scalar& rhs) noexcept
		{
			m_real *= rhs;
			m_imaginary *= rhs;
			return *this;
		}
		constexpr inline Quaternion& operator/=(const Scalar& rhs) noexcept
		{
			m_real /= rhs;
			m_imaginary /= rhs;
			return *this;
		}
		constexpr inline Scalar dot(const Quaternion& rhs) noexcept
		{
			return m_imaginary.dot(rhs.m_imaginary) + m_real * rhs.m_real;
		}
		constexpr friend inline Scalar dot(Quaternion& lhs, Quaternion& rhs) noexcept
		{
			//return lhs.m_imaginary.dot(rhs.m_imaginary) + lhs.m_real * rhs.m_real;
			return lhs.m_imaginary.dot(rhs.m_imaginary) + lhs.m_real * rhs.m_real;
		}
		constexpr inline Scalar squared_norm() const noexcept
		{
			return m_real * m_real + m_imaginary.L2_square();
		}
		inline Scalar norm() const noexcept {
			return sqrt(squared_norm());
		}
		inline Quaternion& inversed() noexcept
		{
			Scalar inverse_magnitude = Scalar(1) / norm();
			m_real *= inverse_magnitude;
			m_imaginary = -m_imaginary * inverse_magnitude;
			return *this;
		}
		inline Quaternion inverse() const noexcept
		{
			Quaternion result{};
			Scalar inverse_magnitude = Scalar(1) / norm();
			result.m_real = m_real * inverse_magnitude;
			result.m_imaginary = -m_imaginary * inverse_magnitude;
			return result;
		}
		inline Quaternion& normalized() noexcept
		{
			Scalar inverse_magnitude = Scalar(1) / norm();
			m_real *= inverse_magnitude;
			m_imaginary *= inverse_magnitude;
			return *this;
		}
		inline Quaternion normalize() const noexcept
		{
			Quaternion result;
			Scalar inverse_magnitude = Scalar(1) / norm();
			result.m_real = m_real * inverse_magnitude;
			result.m_imaginary = m_imaginary * inverse_magnitude;
			return result;
		}
		// Order: roll(x), pitch(y), yaw(z)
		// Returns in Degrees
		inline Vec<Scalar, 3> to_euler_angles() noexcept
		{
			//Source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
			Vec<Scalar, 3> result;
			// roll (x)
			Scalar sinr_cosp = static_cast<Scalar>(2.0) * static_cast<Scalar>(m_real * m_imaginary[0] + m_imaginary[1] * m_imaginary[2]);
			Scalar cosr_cosp = static_cast<Scalar>(1.0) - static_cast<Scalar>(2.0) * static_cast<Scalar>(square(m_imaginary[0]) + square(m_imaginary[1]));
			result[0] = Scalar(std::atan2(sinr_cosp, cosr_cosp) * rad_to_deg);

			// pitch (y)
			//
			//Scalar sinp = static_cast<Scalar>(2.0) * static_cast<Scalar>(m_real * m_imaginary[2] + m_imaginary[1] * m_imaginary[0]);
			//if (std::abs(sinp) >= static_cast<Scalar>(0.999))
			//	return Vec<Scalar, 3> { Scalar(std::copysign(2 * std::atan2(m_imaginary[0], m_real), sinp)* rad_to_deg),
			//						Scalar(std::copysign(pi_2, sinp)* rad_to_deg),
			//						0.f};
			//else
			//	result[1] = Scalar(std::asin(sinp) * rad_to_deg);
			Scalar sinp = static_cast<Scalar>(2.0) * static_cast<Scalar>(m_real * m_imaginary[1] - m_imaginary[2] * m_imaginary[0]);
			if (std::abs(sinp) >= static_cast<Scalar>(1.0))
				result[1] = Scalar(std::copysign(pi_2, sinp) * rad_to_deg);
			else
				result[1] = Scalar(std::asin(sinp) * rad_to_deg);
			//auto p = Scalar(2.0) * (m_real * m_imaginary[1] - m_imaginary[0] * m_imaginary[2]);
			//Scalar sinp = std::sqrt(Scalar(1.0) + p);
			//Scalar cosp = std::sqrt(Scalar(1.0) - p);
			//result[1] = Scalar((Scalar(2.0) * std::atan2(sinp, cosp) - Math::pi_2) * rad_to_deg);

			// yaw (z)

			Scalar siny_cosp = static_cast<Scalar>(2.0) * static_cast<Scalar>(m_real * m_imaginary[2] + m_imaginary[1] * m_imaginary[0]);
			Scalar cosy_cosp = static_cast<Scalar>(1.0) - static_cast<Scalar>(2.0) * static_cast<Scalar>(square(m_imaginary[1]) + square(m_imaginary[2]));
			result[2] = Scalar(std::atan2(siny_cosp, cosy_cosp) * rad_to_deg);

			return result;
			//float x = m_imaginary[0];
			//float y = m_imaginary[1];
			//float z = m_imaginary[2];
			//float w = m_real;
			//auto t0 = 2.0 * (w * x + y * z);
			//auto t1 = 1.0 - 2.0 * (x * x + y * y);
			//auto roll_x = std::atan2(t0, t1) * rad_to_deg;

			//auto t2 = 2.0 * (w * y - z * x);
			//if (t2 > 1.0)
			//	t2 = 1.0;
			//if (t2 < -1.0)
			//	t2 = -1.0;
			//auto pitch_y = std::asin(t2) * rad_to_deg;

			//auto t3 = 2.0 * (w * z + x * y);
			//auto t4 = 1.0 - 2.0 * (y * y + z * z);
			//auto yaw_z = std::atan2(t3, t4) * rad_to_deg;

			//return Vec<Scalar, 3>{ roll_x, pitch_y, yaw_z };
		}
	private:
		Scalar m_real;
		Vec<Scalar, 3> m_imaginary;
	};
	
}