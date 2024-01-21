module;

#include <cassert>
#include <cmath>
#include <array>
#include <string>

export module NYANMath:Matrix;

import :Util;
import :Constants;
import :Vector;

export namespace Math
{

	template<ScalarT Scalar, size_t Size> class Vec;
	template<ScalarT Scalar> class Quaternion;

	//Type, rows, columns, column_major
	template<
		ScalarT Scalar,
		size_t Size_y, //height, row_count, n rows
		size_t Size_x, //width, column_count, n columns
		bool column_major = true>
	class Mat
	{
	public:
		using value_type = Scalar;
		constexpr Mat() noexcept :
			m_data()
		{
		}

		constexpr explicit Mat(const Scalar& scalar) noexcept :
			m_data()
		{
			for (size_t i = 0; i < Size_x * Size_y; i++)
				m_data[i] = scalar;
		}
		constexpr explicit Mat(const Quaternion<Scalar>& quaternion) noexcept :
			m_data()
		{
			assert((Size_x == 3 || Size_x == 4) && (Size_y == 3 || Size_y == 4) && "Rotation matrices from Quaternions are inherently 3D or 3D homogeneous");
			Scalar s2 = Scalar(2) / quaternion.squared_norm();
			//auto normQuat = quaternion.normalize();

			Scalar qr = quaternion.m_real;
			Scalar qi = quaternion.m_imaginary[0];
			Scalar qj = quaternion.m_imaginary[1];
			Scalar qk = quaternion.m_imaginary[2];

			at(0, 0) = 1 - s2 * (qj * qj + qk * qk);
			at(0, 1) = s2 * (qi * qj + qk * qr);
			at(0, 2) = s2 * (qi * qk - qj * qr);

			at(1, 0) = s2 * (qi * qj - qk * qr);
			at(1, 1) = 1 - s2 * (qi * qi + qk * qk);
			at(1, 2) = s2 * (qj * qk + qi * qr);

			at(2, 0) = s2 * (qi * qk + qj * qr);
			at(2, 1) = s2 * (qj * qk - qi * qr);
			at(2, 2) = 1 - s2 * (qi * qi + qj * qj);


			if constexpr (Size_x == 4) {
				at(3, 0) = Scalar(0);
				at(3, 1) = Scalar(0);
				at(3, 2) = Scalar(0);
			}
			if constexpr (Size_y == 4) {
				at(0, 3) = Scalar(0);
				at(1, 3) = Scalar(0);
				at(2, 3) = Scalar(0);
			}
			if constexpr (Size_x == 4 && Size_x == 4) {
				at(3, 3) = Scalar(1);
			}
		}
		constexpr explicit Mat(const Vec<Scalar, Size_x>& vec) noexcept :
			m_data()
		{
			assert(Size_y == 1 || Size_x == 1);
			if constexpr (Size_y == 1) {
				for (size_t i = 0; i < Size_x; i++)
					m_data[i] = vec[i];
			}
			else if constexpr (Size_x == 1) {
				for (size_t i = 0; i < Size_y; i++)
					m_data[i] = vec[i];
			}
		}
		template<typename T>
		constexpr explicit Mat(const T(&list)[Size_x * Size_y]) noexcept :
		m_data()
		{
			for (size_t y = 0; y < Size_y; y++) {
				for (size_t x = 0; x < Size_x; x++) {
					//Memory layout differs from user expectation
					if constexpr (column_major)
						at(x, y) = Scalar(list[Math::at<Size_y, column_major>(x, y)]);
					else
						at(x, y) = Scalar(list[Math::at<Size_x, column_major>(x, y)]);
				}
			}
		}
		//template<typename T>
		//constexpr explicit Mat(const std::vector<T>& vec) : m_data() {
		//	for (size_t y = 0; y < Size_y; y++) {
		//		for (size_t x = 0; x < Size_x; x++) {
		//			//Memory layout differs from user expectation
		//			if constexpr (column_major)
		//				at(x, y) = Scalar(vec[Math::at<Size_y, column_major>(x, y)]);
		//			else
		//				at(x, y) = Scalar(vec[Math::at<Size_x, column_major>(x, y)]);
		//		}
		//	}
		//}
		template<ScalarT... Args>
		constexpr explicit Mat(Args... args) noexcept {
			static_assert(sizeof...(Args) == (Size_x * Size_y) || sizeof...(Args) == 1, "Invalid amount of parameters for this mat");
			if constexpr (sizeof...(Args) == 1) {
				for (size_t i = 0; i < (Size_x * Size_y); i++)
				{
					std::array<std::common_type_t<Args...>, 1> list = { std::forward<Args>(args)... };
					m_data[i] = static_cast<Scalar>(list[0]);
				}
			}
			else {
				std::array<Scalar, sizeof...(Args)> list = { static_cast<Scalar>(args)... };
				for (size_t y = 0; y < Size_y; y++) {
					for (size_t x = 0; x < Size_x; x++) {
						at(x, y) = Scalar{ list[Math::at<Size_x, false>(x, y)] };
					}
				}
			}
		}
		template<typename ScalarOther, size_t Size_x_other, size_t Size_y_other, bool column_major_other>
		constexpr explicit Mat(const Mat<ScalarOther, Size_x_other, Size_y_other, column_major_other>& other) noexcept :
			m_data()
		{
			for (size_t y = 0; y < min(Size_y, Size_y_other); y++) {
				for (size_t x = 0; x < min(Size_x, Size_x_other); x++) {
					at(x, y) = other.at(x, y);
				}
			}
		}
		constexpr friend inline Mat operator+(const Mat& lhs, const Mat& rhs) noexcept {
			Mat result;
			for (size_t i = 0; i < Size_x * Size_y; i++)
				result.m_data[i] = lhs.m_data[i] + rhs.m_data[i];
			return result;
		}
		constexpr friend inline Mat operator-(const Mat& lhs, const Mat& rhs) noexcept {
			Mat result;
			for (size_t i = 0; i < Size_x * Size_y; i++)
				result.m_data[i] = lhs.m_data[i] - rhs.m_data[i];
			return result;
		}
		constexpr friend inline Mat operator+(const Mat& lhs, const Scalar& rhs) noexcept {
			Mat result;
			for (size_t i = 0; i < Size_x * Size_y; i++)
				result.m_data[i] = lhs.m_data[i] + rhs;
			return result;
		}
		constexpr friend inline Mat operator-(const Mat& lhs, const Scalar& rhs) noexcept {
			Mat result;
			for (size_t i = 0; i < Size_x * Size_y; i++)
				result.m_data[i] = lhs.m_data[i] - rhs;
			return result;
		}
		constexpr friend inline Mat operator*(const Mat& lhs, const Scalar& rhs) noexcept {
			Mat result;
			for (size_t i = 0; i < Size_x * Size_y; i++)
				result.m_data[i] = lhs.m_data[i] * rhs;
			return result;
		}
		constexpr friend inline Mat operator*(const Scalar& lhs, const Mat& rhs) noexcept {
			return rhs * lhs;
		}
		constexpr friend inline Mat operator/(const Mat& lhs, const Scalar& rhs) noexcept {
			Mat result;
			for (size_t i = 0; i < Size_x * Size_y; i++)
				result.m_data[i] = lhs.m_data[i] / rhs;
			return result;
		}
		constexpr friend inline Vec<Scalar, Size_y> operator*(const Mat& lhs, const Vec<Scalar, Size_x>& rhs) noexcept {
			Vec<Scalar, Size_y> result;
			for (size_t y = 0; y < Size_y; y++) {
				Scalar tmp = Scalar(0);
				for (size_t x = 0; x < Size_x; x++)
					tmp += lhs(x, y) * rhs[x];
				result[y] = tmp;
			}
			return result;
		}
		constexpr friend inline bool operator==(const Mat& lhs, const Mat& rhs) noexcept {
			for (size_t i = 0; i < Size_x * Size_y; i++)
				if (lhs.m_data[i] != rhs.m_data[i])
					return false;
			return true;
		}
		constexpr friend inline bool operator==(const Mat& lhs, const Scalar& rhs) noexcept {
			for (size_t i = 0; i < Size_x * Size_y; i++)
				if (lhs.m_data[i] != rhs)
					return false;
			return true;
		}
		constexpr friend inline bool close(const Mat& lhs, const Scalar& rhs, const Scalar& eps = Scalar(1e-5)) noexcept  {
			for (size_t i = 0; i < Size_x * Size_y; i++)
				if (!close(lhs.m_data[i], rhs, eps))
					return false;
			return true;
		}
		constexpr inline std::string convert_to_string() const 
		{
			std::string result("(");
			for (size_t y = 0; y < (Size_y - 1); y++) {
				for (size_t x = 0; x < Size_x; x++)
					result += std::to_string(at(x, y)) + ", ";
				result += "\n";
			}
			for (size_t x = 0; x < Size_x - 1; x++)
				result += std::to_string(at(x, Size_y - 1)) + ", ";
			result += std::to_string(m_data[Size_x * Size_y - 1]) + ")";
			return result;
		}
		constexpr inline Mat& operator=(const Scalar& rhs) noexcept
		{
			for (size_t i = 0; i < Size_x * Size_y; i++)
				m_data[i] += rhs;
			return *this;
		}
		constexpr inline Mat& operator=(Scalar& rhs) noexcept
		{
			for (size_t i = 0; i < Size_x * Size_y; i++)
				m_data[i] = rhs;
			return *this;
		}
		constexpr inline Mat& operator+=(const Mat& rhs) noexcept
		{
			for (size_t i = 0; i < Size_x * Size_y; i++)
				m_data[i] += rhs.m_data[i];
			return *this;
		}
		constexpr inline Mat& operator-=(const Mat& rhs) noexcept
		{
			for (size_t i = 0; i < Size_x * Size_y; i++)
				m_data[i] -= rhs.m_data[i];
			return *this;
		}

		//constexpr inline Mat& operator*=(const Mat& rhs);
		constexpr inline Mat& operator+=(const Scalar& rhs) noexcept
		{
			for (size_t i = 0; i < Size_x * Size_y; i++)
				m_data[i] += rhs;
			return *this;
		}

		constexpr inline Mat& operator-=(const Scalar& rhs) noexcept
		{
			for (size_t i = 0; i < Size_x * Size_y; i++)
				m_data[i] -= rhs;
			return *this;
		}

		constexpr inline Mat& operator*=(const Scalar& rhs) noexcept
		{
			for (size_t i = 0; i < Size_x * Size_y; i++)
				m_data[i] *= rhs;
			return *this;
		}
		constexpr inline Mat& operator/=(const Scalar& rhs) noexcept
		{ 
			for (size_t i = 0; i < Size_x * Size_y; i++)
				m_data[i] /= rhs;
			return *this;
		}
		constexpr inline Vec<Scalar, Size_x> row(const size_t index) const noexcept
		{
			assert(Size_y > index);
			//if (Size_y <= index)
			//	throw std::out_of_range("Index out of range");
			if constexpr (!column_major) {
				return m_vectors[index];
			}
			else {
				Vec<Scalar, Size_x> ret;
				for (size_t x = 0; x < Size_x; x++)
					ret[x] = at(x, index);
				return ret;
			}
		}
		constexpr inline Vec<Scalar, Size_y> col(const size_t index) const noexcept
		{
			assert(Size_x > index);
			//if (Size_x <= index)
			//	throw std::out_of_range("Index out of range");
			if constexpr (column_major) {
				return m_vectors[index];
			}
			else {
				Vec<Scalar, Size_y> ret;
				for (size_t y = 0; y < Size_y; y++)
					ret[y] = at(index, y);
				return ret;
			}
		}
		constexpr inline const Scalar& operator()(const size_t col, const size_t  row) const noexcept
		{
			if constexpr (column_major) {
				assert(Size_x * Size_y > col * Size_y + row);
			}
			else {
				assert(Size_x * Size_y > row * Size_x + col);
			}
			//Just pass through, User responsible for bounds
			//if (Size_x * Size_y <= index)
			//	throw std::out_of_range("Index out of range");
			return at(col, row);
		}
		constexpr inline Scalar& operator()(const size_t col, const size_t row) noexcept
		{
			if constexpr (column_major) {
				assert(Size_x * Size_y > col * Size_y + row);
			}
			else {
				assert(Size_x * Size_y > row * Size_x + col);
			}
			//Just pass through, User responsible for bounds
			//if (Size_x * Size_y <= index)
			//	throw std::out_of_range("Index out of range");
			return at(col, row);
		}
		constexpr inline Mat& transposed() noexcept
		{
			assert(Size_x == Size_y);
			for (size_t y = 0; y < Size_y; y++) {
				for (size_t x = y + 1; x < Size_x; x++) {
					std::swap(at(x, y), at(y, x));
				}
			}
			return *this;
		}
		constexpr inline Mat transpose() const noexcept
		{
			Mat<Scalar, Size_y, Size_x, column_major> ret;
			for (size_t y = 0; y < Size_y; y++) {
				for (size_t x = y; x < Size_x; x++) {
					ret(x, y) = at(y, x);
					ret(y, x) = at(x, y);
				}
			}
			return ret;
		}

		constexpr inline Scalar determinante() const noexcept
		{
			assert(Size_x == Size_y);
			if constexpr (Size_x == 2) {
				return (m_data[0] * m_data[3] - m_data[1] * m_data[2]);
			}
			else if constexpr (Size_x == 3) {
				Scalar determinante = m_data[0] * (m_data[4] * m_data[8] - m_data[5] * m_data[7]); //a * (ei - fh)
				determinante += m_data[1] * (m_data[5] * m_data[6] - m_data[3] * m_data[8]); //+ b * (fg - di)
				determinante += m_data[2] * (m_data[3] * m_data[7] - m_data[4] * m_data[6]); //+ c * (dh - eg)
				return determinante;
			}
			else if constexpr (Size_x == 4) {
				/*
				* [11 12 13 14] [ 0  1  2  3]
				* [21 22 23 24] [ 4  5  6  7]
				* [31 32 33 34] [ 8  9 10 11]
				* [41 42 43 44] [12 13 14 15]
				*/
				auto a11 = at(0, 0);
				auto a12 = at(0, 1);
				auto a13 = at(0, 2);
				auto a14 = at(0, 3);
				auto a21 = at(1, 0);
				auto a22 = at(1, 1);
				auto a23 = at(1, 2);
				auto a24 = at(1, 3);
				auto a31 = at(2, 0);
				auto a32 = at(2, 1);
				auto a33 = at(2, 2);
				auto a34 = at(2, 3);
				auto a41 = at(3, 0);
				auto a42 = at(3, 1);
				auto a43 = at(3, 2);
				auto a44 = at(3, 3);
				auto a33a44a43a34 = a33 * a44 - a43 * a34;
				auto a32a44a42a34 = a32 * a44 - a42 * a34;
				auto a32a43a42a33 = a32 * a43 - a42 * a33;
				auto a31a44a41a34 = a31 * a44 - a41 * a34;
				auto a31a43a41a33 = a31 * a43 - a41 * a33;
				auto a31a42a41a32 = a31 * a42 - a41 * a32;
				auto f1 = (a22 * a33a44a43a34 - a23 * a32a44a42a34 + a24 * a32a43a42a33);
				auto f2 = -(a21 * a33a44a43a34 - a23 * a31a44a41a34 + a24 * a31a43a41a33);
				auto f3 = (a21 * a32a44a42a34 - a22 * a31a44a41a34 + a24 * a31a42a41a32);
				auto f4 = -(a21 * a32a43a42a33 - a22 * a31a43a41a33 + a23 * a31a42a41a32);
				return static_cast<Scalar>(a11 * f1 + a12 * f2 + a13 * f3 + a14 * f4);
			}
			else {
				return Scalar(0);
			}
		}
		constexpr inline bool inverse(Mat& res) const noexcept {
			Scalar determinante;
			assert(Size_x == Size_y);
			if constexpr (Size_x == 2) {
				determinante = this->determinante();
				if (close(determinante, Scalar(0)))
					return false;
				determinante = Scalar(1) / determinante;
				res.m_data[0] = determinante * m_data[3];
				res.m_data[1] = -determinante * m_data[1];
				res.m_data[2] = -determinante * m_data[2];
				res.m_data[3] = determinante * m_data[0];
				return true;
			}
			else if constexpr (Size_x == 3) {
				determinante = this->determinante();
				if (close(determinante, Scalar(0)))
					return false;
				determinante = Scalar(1) / determinante;
				/*
				* [ 0 1 2 ]
				* [ 3 4 5 ]
				* [ 6 7 8 ]
				*/
				res.m_data[0] = determinante * (m_data[4] * m_data[8] - m_data[5] * m_data[7]);
				res.m_data[3] = -determinante * (m_data[3] * m_data[8] - m_data[5] * m_data[6]);
				res.m_data[6] = determinante * (m_data[3] * m_data[7] - m_data[4] * m_data[6]);
				res.m_data[1] = -determinante * (m_data[1] * m_data[8] - m_data[7] * m_data[2]);
				res.m_data[4] = determinante * (m_data[0] * m_data[8] - m_data[2] * m_data[6]);
				res.m_data[7] = -determinante * (m_data[0] * m_data[7] - m_data[1] * m_data[6]);
				res.m_data[2] = determinante * (m_data[1] * m_data[5] - m_data[2] * m_data[4]);
				res.m_data[5] = -determinante * (m_data[0] * m_data[5] - m_data[2] * m_data[3]);
				res.m_data[8] = determinante * (m_data[0] * m_data[4] - m_data[1] * m_data[3]);
				return true;
			}
			else if constexpr (Size_x == 4) {
				determinante = this->determinante();
				if (close(determinante, Scalar(0)))
					return false;
				determinante = Scalar(1) / determinante;
				//Currently not working
				auto a11 = at(0, 0);
				auto a12 = at(1, 0);
				auto a13 = at(2, 0);
				auto a14 = at(3, 0);
				auto a21 = at(0, 1);
				auto a22 = at(1, 1);
				auto a23 = at(2, 1);
				auto a24 = at(3, 1);
				auto a31 = at(0, 2);
				auto a32 = at(1, 2);
				auto a33 = at(2, 2);
				auto a34 = at(3, 2);
				auto a41 = at(0, 3);
				auto a42 = at(1, 3);
				auto a43 = at(2, 3);
				auto a44 = at(3, 3);
				res.at(0, 0) = determinante * (a22 * a33 * a44 + a23 * a34 * a42 + a24 * a32 * a43
					- a24 * a33 * a42 - a23 * a32 * a44 - a22 * a34 * a43);
				res.at(1, 0) = determinante * (-a12 * a33 * a44 - a13 * a34 * a42 - a14 * a32 * a43
					+ a14 * a33 * a42 + a13 * a32 * a44 + a12 * a34 * a43);
				res.at(2, 0) = determinante * (a12 * a23 * a44 + a13 * a24 * a42 + a14 * a22 * a43
					- a14 * a23 * a42 - a13 * a22 * a44 - a12 * a24 * a43);
				res.at(3, 0) = determinante * (-a12 * a23 * a34 - a13 * a24 * a32 - a14 * a22 * a33
					+ a14 * a23 * a32 + a13 * a22 * a34 + a12 * a24 * a33);

				res.at(0, 1) = determinante * (-a21 * a33 * a44 - a23 * a34 * a41 - a24 * a31 * a43
					+ a24 * a33 * a41 + a23 * a31 * a44 + a21 * a34 * a43);
				res.at(1, 1) = determinante * (a11 * a33 * a44 + a13 * a34 * a41 + a14 * a31 * a43
					- a14 * a33 * a41 - a13 * a31 * a44 - a11 * a34 * a43);
				res.at(2, 1) = determinante * (-a11 * a23 * a44 - a13 * a24 * a41 - a14 * a21 * a43
					+ a14 * a23 * a41 + a13 * a21 * a44 + a11 * a24 * a43);
				res.at(3, 1) = determinante * (a11 * a23 * a34 + a13 * a24 * a31 + a14 * a21 * a33
					- a14 * a23 * a31 - a13 * a21 * a34 - a11 * a24 * a33);

				res.at(0, 2) = determinante * (a21 * a32 * a44 + a22 * a34 * a41 + a24 * a31 * a42
					- a24 * a32 * a41 - a22 * a31 * a44 - a21 * a34 * a42);
				res.at(1, 2) = determinante * (-a11 * a32 * a44 - a12 * a34 * a41 - a14 * a31 * a42
					+ a14 * a32 * a41 + a12 * a31 * a44 + a11 * a34 * a42);
				res.at(2, 2) = determinante * (a11 * a22 * a44 + a12 * a24 * a41 + a14 * a21 * a42
					- a14 * a22 * a41 - a12 * a21 * a44 - a11 * a24 * a42);
				res.at(3, 2) = determinante * (-a11 * a22 * a34 - a12 * a24 * a31 - a14 * a21 * a32
					+ a14 * a22 * a31 + a12 * a21 * a34 + a11 * a24 * a32);

				res.at(0, 3) = determinante * (-a21 * a32 * a43 - a22 * a33 * a41 - a23 * a31 * a42
					+ a23 * a32 * a41 + a22 * a31 * a43 + a21 * a33 * a42);
				res.at(1, 3) = determinante * (a11 * a32 * a43 + a12 * a33 * a41 + a13 * a31 * a42
					- a13 * a32 * a41 - a12 * a31 * a43 - a11 * a33 * a42);
				res.at(2, 3) = determinante * (-a11 * a22 * a43 - a12 * a23 * a41 - a13 * a21 * a42
					+ a13 * a22 * a41 + a12 * a21 * a43 + a11 * a23 * a42);
				res.at(3, 3) = determinante * (a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32
					- a13 * a22 * a31 - a12 * a21 * a33 - a11 * a23 * a32);
				return true;
			}
			else {
				return false;
			}
		}
		constexpr inline Vec<Scalar, 3> euler() const noexcept
		{
			assert(Size_x > 2 && Size_y > 2);
			assert(Size_x < 5 && Size_y < 5);
			//Math::vec3 tmpX{ col(0) };
			//Math::vec3 tmpY{ col(1) };
			//Math::vec3 tmpZ{ col(2) };
			if (Math::close(col(0)[2], Scalar(-1.0f))) {
				return Vec<Scalar, 3>{ Math::pi_2* Math::rad_to_deg,
					Math::pi_2* Math::rad_to_deg,
					std::atan2(col(1)[0], -col(1)[1])* Math::rad_to_deg };
			}
			else if (Math::close(col(0)[2], Scalar(1.0f))) {
				return Vec<Scalar, 3>{ Math::pi_2* Math::rad_to_deg,
					-Math::pi_2 * Math::rad_to_deg,
					std::atan2(col(1)[0], -col(1)[1])* Math::rad_to_deg };
			}
			else {
				return Vec<Scalar, 3>{ std::atan2(col(1)[2], col(2)[2])* Math::rad_to_deg,
					std::atan2(-col(0)[2], std::sqrt(col(1)[2] * col(1)[2] + col(2)[2] * col(2)[2]))* Math::rad_to_deg,
					std::atan2(col(0)[1], col(0)[0])* Math::rad_to_deg };
			}
		}

		constexpr static Mat eye(const Scalar& val) noexcept
		{
			Mat ret{ 0 };
			for (size_t i = 0; i < min(Size_x, Size_y); i++)
				ret.at(i, i) = val;
			return ret;
		}
		constexpr inline static Mat identity() noexcept
		{
			return Mat::eye(1);
		}
		template<size_t Size>
		constexpr void set_col(Vec<Scalar, Size> column, size_t colNum = 0, size_t offset = 0) noexcept
		{
			assert(colNum < Size_x);
			assert(offset + Size <= Size_y);
			for (size_t i = offset; i < Size + offset; i++)
				at(colNum, i) = column[i];
		}
		template<size_t Size>
		constexpr void set_row(Vec<Scalar, Size> row, size_t rowNum = 0, size_t offset = 0) noexcept
		{
			assert(rowNum < Size_y);
			assert(offset + Size <= Size_x);
			for (size_t i = offset; i < Size + offset; i++)
				at(i, rowNum) = row[i];
		}
		constexpr void set_to_identity() noexcept
		{
			for (size_t i = 0; i < Size_y * Size_x; i++)
				m_data[i] = 0;
			for (size_t i = 0; i < min(Size_x, Size_y); i++)
				at(i, i) = 1;
		}

		/*	Rotates first around Z, then around Y and then around Z
		*	yaw (Z) , pitch (Y), roll (X) in degrees
		*/
		static Mat<Scalar, 3, 3, column_major> rotation_matrix(Scalar roll, Scalar pitch, Scalar yaw) noexcept
		{

			// roll		[x]
			// pitch	[y]
			// yaw		[z]
			//TODO use doubles until return for more precision
			auto cy = cos(yaw * deg_to_rad);
			auto sy = sin(yaw * deg_to_rad);
			auto cp = cos(pitch * deg_to_rad);
			auto sp = sin(pitch * deg_to_rad);
			auto cr = cos(roll * deg_to_rad);
			auto sr = sin(roll * deg_to_rad);
			Mat<Scalar, 3, 3, column_major> mat;
			mat(0, 0) = static_cast<Scalar>(cy * cp);
			mat(1, 0) = static_cast<Scalar>(cy * sp * sr - sy * cr);
			mat(2, 0) = static_cast<Scalar>(cy * sp * cr + sy * sr);
			mat(0, 1) = static_cast<Scalar>(sy * cp);
			mat(1, 1) = static_cast<Scalar>(sy * sp * sr + cr * cy);
			mat(2, 1) = static_cast<Scalar>(sy * sp * cr - cy * sr);
			mat(0, 2) = static_cast<Scalar>(-sp);
			mat(1, 2) = static_cast<Scalar>(cp * sr);
			mat(2, 2) = static_cast<Scalar>(cp * cr);

			return mat;
		}

		static Mat<Scalar, 4, 4, column_major> affine_transformation_matrix(Vec<Scalar, 3>  roll_pitch_yaw, Vec<Scalar, 3>  translation_vector) noexcept
		{	// roll (x), pitch (y), yaw (z)
			Mat<Scalar, 4, 4, column_major> mat(Mat<Scalar, 3, 3, column_major>::rotation_matrix(roll_pitch_yaw));
			mat.set_col(translation_vector, 3);
			mat(3, 3) = 1;
			return mat;
		}
		static Mat<Scalar, 4, 4, column_major> affine_transformation_matrix(Vec<Scalar, 3>  roll_pitch_yaw, Vec<Scalar, 3>  translation_vector, Vec<Scalar, 3>  scale)noexcept
		{
			Mat<Scalar, 4, 4, column_major> mat = Math::Mat<Scalar, Size_y, Size_x, column_major>::affine_transformation_matrix(roll_pitch_yaw, translation_vector);

			for (size_t col = 0; col < 3; ++col)
				for (size_t row = 0; row < 3; ++row)
					mat(col, row) *= scale[col];
			return mat;
		}
		static Mat<Scalar, 4, 4, column_major> affine_transformation_matrix(Quaternion<Scalar>  orientation, Vec<Scalar, 3>  translation_vector) noexcept
		{
			Mat<Scalar, 4, 4, column_major> mat{ orientation };
			mat.set_col(translation_vector, 3);
			mat(3, 3) = 1;
			return mat;
		}
		static Mat<Scalar, 4, 4, column_major> affine_transformation_matrix(Quaternion<Scalar>  orientation, Vec<Scalar, 3>  translation_vector, Vec<Scalar, 3>  scale)noexcept
		{
			Mat<Scalar, 4, 4, column_major> mat = affine_transformation_matrix(orientation, translation_vector);

			for (size_t col = 0; col < 3; ++col)
				for (size_t row = 0; row < 3; ++row)
					mat(col, row) *= scale[col];
			return mat;
		}

		inline Mat<Scalar, Size_y, Size_x, column_major> inverse_affine_transformation_matrix() noexcept
		{
			assert(Size_x == 4);
			assert(Size_y == 4 || Size_y == 3);
			Mat<Scalar, Size_y, Size_x, column_major> mat;
			if constexpr ((Size_y == 4) && (Size_x == 4)) {
				for (size_t row = 0; row < 3; row++) {
					for (size_t col = 0; col < 3; col++) {
						mat.at(col, row) = at(row, col);
					}
				}
				mat.set_col(mat * -col(3), 3);
				mat(3, 3) = 1;
			}
			return mat;
		}
		static Mat<Scalar, 3, 3, column_major> rotation_matrix(Vec<Scalar, 3>  roll_pitch_yaw) noexcept
		{
			return rotation_matrix(roll_pitch_yaw[0], roll_pitch_yaw[1], roll_pitch_yaw[2]);
		}
		static Mat<Scalar, 4, 4, column_major> translation_matrix(Vec<Scalar, 3>  translation_vector) noexcept
		{
			Mat<Scalar, 4, 4, column_major> matrix = Mat<Scalar, 4, 4, column_major>::identity();
			matrix.set_col(translation_vector, 3);
			return matrix;
		}
		static Mat<Scalar, 4, 4, column_major> look_at(Vec<Scalar, 3> eye, Vec<Scalar, 3> at, Vec<Scalar, 3> up) noexcept
		{
			auto matrix = Mat<Scalar, 4, 4, column_major>::identity();
			Vec<Scalar, 3> zAxis = at - eye;
			zAxis.normalize();
			Vec<Scalar, 3> xAxis = zAxis.cross(up);
			xAxis.normalize();
			Vec<Scalar, 3> yAxis = zAxis.cross(xAxis);
			//matrix.set_row(xAxis, 0);
			//matrix.set_row(yAxis, 1);
			//matrix.set_row(-zAxis, 2);
			matrix.at(0, 0) = xAxis.x();
			matrix.at(1, 0) = xAxis.y();
			matrix.at(2, 0) = xAxis.z();
			matrix.at(0, 1) = yAxis.x();
			matrix.at(1, 1) = yAxis.y();
			matrix.at(2, 1) = yAxis.z();
			matrix.at(0, 2) = -(zAxis.x());
			matrix.at(1, 2) = -(zAxis.y());
			matrix.at(2, 2) = -(zAxis.z());
			matrix.at(3, 0) = -(xAxis.dot(eye));
			matrix.at(3, 1) = -(yAxis.dot(eye));
			matrix.at(3, 2) = (zAxis.dot(eye));
			return matrix;
		}
		static Mat<Scalar, 4, 4, column_major> first_person(Vec<Scalar, 3> eye, Vec<Scalar, 3> direction, Vec<Scalar, 3> up, Vec<Scalar, 3> right) noexcept
		{
			auto matrix = Mat<Scalar, 4, 4, column_major>::identity();
			Vec<Scalar, 3> zAxis = direction;
			Vec<Scalar, 3> xAxis = right;
			Vec<Scalar, 3> yAxis = up;
			matrix.set_row(xAxis, 0);
			matrix.set_row(yAxis, 1);
			matrix.set_row(zAxis, 2);
			//matrix.at(0, 0) = xAxis.x();
			//matrix.at(1, 0) = xAxis.y();
			//matrix.at(2, 0) = xAxis.z();
			//matrix.at(0, 1) = yAxis.x();
			//matrix.at(1, 1) = yAxis.y();
			//matrix.at(2, 1) = yAxis.z();
			//matrix.at(0, 2) = -(zAxis.x());
			//matrix.at(1, 2) = -(zAxis.y());
			//matrix.at(2, 2) = -(zAxis.z());
			matrix.at(3, 0) = (-xAxis.dot(eye));
			matrix.at(3, 1) = (-yAxis.dot(eye));
			matrix.at(3, 2) = (-zAxis.dot(eye));
			return matrix;
		}
		static Mat<Scalar, 4, 4, column_major> perspectiveY(Scalar nearPlane, Scalar farPlane, Scalar fovY, Scalar aspectRatio) noexcept
		{	//Right handed, zero to one
			Mat<Scalar, 4, 4, column_major> matrix{};
			const Scalar tanHalfFovy = static_cast<Scalar>(tan(static_cast<double>(fovY) * 0.5 * deg_to_rad));
			//matrix(0, 0) = Scalar(1) / ( aspect * tanHalfFovy);
			//matrix(1, 1) = Scalar(1) / (tanHalfFovy);
			//matrix(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
			//matrix(2, 3) = -Scalar(1);
			//matrix(3, 2) = -(Scalar(2) * farPlane * nearPlane) / (farPlane - nearPlane);
			matrix.at(0, 0) = static_cast<Scalar>(1) / (aspectRatio * tanHalfFovy);
			matrix.at(1, 1) = static_cast<Scalar>(1) / (tanHalfFovy);
			matrix.at(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
			matrix.at(2, 3) = -Scalar(1);
			matrix.at(3, 2) = -Scalar(2) * (nearPlane * farPlane) / (farPlane - nearPlane);
			return matrix;
		}
		/// <summary>
		/// Converts  +y|_+x   into vulkan space +z/_+x
		///			 +z/					      |+y
		/// I.e. is left-handed
		/// Also maps onto z [0, 1]
		/// </summary>
		/// <param name="nearPlane"></param>
		/// <param name="farPlane"></param>
		/// <param name="fovX"></param>
		/// <param name="aspectRatio"></param>
		/// <returns></returns>
		static Mat<Scalar, 4, 4, column_major> perspectiveInverseDepthFovXLH(Scalar nearPlane, Scalar farPlane, Scalar fovX, Scalar aspectRatio) noexcept
		{	//Right handed, zero to one
			Mat<Scalar, 4, 4, column_major> matrix;
			auto fovRad = static_cast<double>(fovX) * 0.5 * deg_to_rad;
			const Scalar focalLength = static_cast<Scalar>(tan(fovRad));
			Scalar a = nearPlane / (farPlane - nearPlane);
			Scalar b = farPlane * a;

			matrix(0, 0) = (focalLength);
			matrix(1, 1) = -(focalLength * aspectRatio);
			matrix.at(2, 2) = a;
			matrix.at(2, 3) = -Scalar(1);
			matrix.at(3, 2) = b;
			//matrix(0, 0) = static_cast<Scalar>(1) / (focalLength);
			//matrix(1, 1) = static_cast<Scalar>(1) / (aspectRatio * focalLength);
			//matrix.at(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
			//matrix.at(2, 3) = -Scalar(1);
			//matrix.at(3, 2) = -Scalar(2) * (nearPlane * farPlane) / (farPlane - nearPlane);
			return matrix;
		}
		static Mat<Scalar, 4, 4, column_major> perspectiveFovXLH(Scalar nearPlane, Scalar farPlane, Scalar fovX, Scalar aspectRatio) noexcept
		{
			//Right handed, zero to one
			Mat<Scalar, 4, 4, column_major> matrix;
			auto fovRad = static_cast<double>(fovX) * 0.5 * deg_to_rad;
			const Scalar focalLength = static_cast<Scalar>(tan(fovRad));
			Scalar a = -(farPlane) / (farPlane - nearPlane);
			Scalar b = -farPlane * nearPlane / (farPlane - nearPlane);

			matrix(0, 0) = (focalLength);
			matrix(1, 1) = -(focalLength * aspectRatio);
			matrix.at(2, 2) = a;
			matrix.at(2, 3) = -static_cast<Scalar>(1);
			matrix.at(3, 2) = b;
			//matrix(0, 0) = static_cast<Scalar>(1) / (focalLength);
			//matrix(1, 1) = static_cast<Scalar>(1) / (aspectRatio * focalLength);
			//matrix.at(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
			//matrix.at(2, 3) = -Scalar(1);
			//matrix.at(3, 2) = -Scalar(2) * (nearPlane * farPlane) / (farPlane - nearPlane);
			return matrix;
		}
		static Mat<Scalar, 4, 4, column_major> perspectiveInverseDepthFovXLH(Scalar nearPlane, Scalar fovX, Scalar aspectRatio) noexcept
		{
			//Right handed, zero to one
			Mat<Scalar, 4, 4, column_major> matrix;
			auto fovRad = static_cast<double>(fovX) * 0.5 * deg_to_rad;
			const Scalar focalLength = static_cast<Scalar>(tan(fovRad));
			Scalar a = static_cast<Scalar>(0);
			Scalar b = nearPlane;

			matrix(0, 0) = (focalLength);
			matrix(1, 1) = -(focalLength * aspectRatio);
			matrix.at(2, 2) = a;
			matrix.at(2, 3) = -Scalar(1);
			matrix.at(3, 2) = b;
			//matrix(0, 0) = static_cast<Scalar>(1) / (focalLength);
			//matrix(1, 1) = static_cast<Scalar>(1) / (aspectRatio * focalLength);
			//matrix.at(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
			//matrix.at(2, 3) = -Scalar(1);
			//matrix.at(3, 2) = -Scalar(2) * (nearPlane * farPlane) / (farPlane - nearPlane);
			return matrix;
		}
		static Mat<Scalar, 4, 4, column_major> perspectiveXY(Scalar nearPlane, Scalar farPlane, Scalar fovX, Scalar fovY) noexcept
		{
			//Right handed, zero to one
			Mat<Scalar, 4, 4, column_major> matrix;
			const Scalar tanHalfFovx = static_cast<Scalar>(tan(static_cast<double>(fovX) * 0.5 * deg_to_rad));
			const Scalar tanHalfFovy = static_cast<Scalar>(tan(static_cast<double>(fovY) * 0.5 * deg_to_rad));
			//matrix(0, 0) = Scalar(1) / ( aspect * tanHalfFovy);
			//matrix(1, 1) = Scalar(1) / (tanHalfFovy);
			//matrix(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
			//matrix(2, 3) = -Scalar(1);
			//matrix(3, 2) = -(Scalar(2) * farPlane * nearPlane) / (farPlane - nearPlane);
			matrix(0, 0) = static_cast<Scalar>(1) / (tanHalfFovx);
			matrix(1, 1) = static_cast<Scalar>(1) / (tanHalfFovy);
			matrix.at(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
			matrix.at(2, 3) = -Scalar(1);
			matrix.at(3, 2) = -Scalar(2) * (nearPlane * farPlane) / (farPlane - nearPlane);
			return matrix;
		}

		constexpr inline Scalar& at(size_t col, size_t row) noexcept {
			if constexpr (column_major) {
				return m_data[col * Size_y + row];
			}
			else {
				return m_data[row * Size_x + col];
			}
		}

		[[nodiscard]] constexpr inline const Scalar& at(size_t col, size_t row) const noexcept {
			if constexpr (column_major) {
				return m_data[col * Size_y + row];
			}
			else {
				return m_data[row * Size_x + col];
			}
		}


	private:
		union {
			std::array<Scalar, Size_x* Size_y> m_data;
			std::array<Vec<Scalar, column_major ? Size_y : Size_x>, column_major ? Size_x : Size_y> m_vectors;
			//Columns if columnmajor, rows otherwise
		};
	};


	template<typename Scalar, size_t Size_x, size_t Size_y>
	constexpr inline bool close(const Mat<Scalar, Size_x, Size_y>& lhs, const Mat<Scalar, Size_x, Size_y>& rhs, const Scalar& eps = Scalar(1e-5)) noexcept {
		for (size_t i = 0; i < Size_x; i++)
			for (size_t j = 0; j < Size_y; j++)
				if (!close(lhs(i, j), rhs(i, j), eps))
					return false;
		return true;
	}

	template<typename Scalar, size_t rows, size_t equal, size_t cols, bool column_major>
	constexpr inline Mat<Scalar, rows, cols, column_major> operator*(const Mat<Scalar, rows, equal, column_major>& lhs, const Mat<Scalar, equal, cols, column_major>& rhs)  noexcept {
		Mat<Scalar, rows, cols, column_major > result{};
		for (size_t col = 0; col < cols; col++) {
			for (size_t row = 0; row < rows; row++) {
				Scalar tmp{ 0 };
				for (size_t i = 0; i < equal; i++) {
					tmp += lhs(i, row) * rhs(col, i);
				}
				result(col, row) = tmp;
			}
		}
		return result;
	}
	template<typename Scalar, size_t rows, size_t equal, size_t cols>
	constexpr inline Mat<Scalar, rows, cols, false> operator*(const Mat<Scalar, rows, equal, false>& lhs, const Mat<Scalar, equal, cols, false>& rhs)noexcept {
		Mat<Scalar, rows, cols, false> result{};
		for (size_t row = 0; row < rows; row++) {
			for (size_t col = 0; col < cols; col++) {
				float tmp{ 0.f };
				for (size_t i = 0; i < equal; i++) {
					tmp += lhs(i, row) * rhs(col, i);
				}
				result(col, row) = tmp;
			}
		}
		return result;
	}
	/*
	template<typename Scalar, size_t rows, size_t equal, size_t cols>
	inline Mat<Scalar, rows, cols> operator*(const Mat<Scalar, rows, equal>& lhs, const Mat<Scalar, equal, cols>& rhs) {
		Mat<Scalar, rows, cols> result;
		for (size_t y = 0; y < cols; y++) {
			for (size_t x = 0; x < rows; x++) {
				Scalar tmp = Scalar(0);
				for (size_t i = 0; i < equal; i++) {
					tmp += lhs[at<rows>(y, i)] * rhs[at<equal>(i, x)];
				}
				result[at<rows>(y, x)] = tmp;
			}
		}
		return result;
	}
	*/
}
