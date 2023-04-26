#include <vector>
#include <complex>
#define PI 3.1415926535897932384626433832795


namespace Signal
{
	class FFT{
	public:
		//Прямое Фурье Преобразование
		static vector<complex<double>> fft(vector<complex<double>> x)
		{
			vector<complex<double>> X;
			int N = x.size();
			if (N == 2)	{
				X.push_back( x[0] + x[1]);
				X.push_back( x[0] - x[1] );
			}
			else {
				vector<complex<double>> x_even, x_odd;

				for (int i = 0; i < N / 2; i++)	{//Разделяем на чет и не чет
					x_even.push_back(x[2 * i]);
					x_odd.push_back(x[2 * i + 1]);
				}
				//Отправляем чет и не чет в рекурсию
				vector<complex<double>> X_even = fft(x_even);
				vector<complex<double>> X_odd = fft(x_odd);

				//Полученные значение записываем обратно
				for (int i = 0; i < N / 2; i++)	{
					X.push_back(X_even[i] + w(i, N) * X_odd[i]);
				}
				for (int i = 0; i < N / 2; i++) {
					X.push_back( X_even[i] - w(i, N) * X_odd[i]);
				}
			}
			return X;
		}

		static vector<complex<double>> fftRev(vector<complex<double>> x)
		{
			int N = x.size();
			x = Conjugate(x);
			x = fft(x);
			x = Conjugate(x);
			for (int i = 0; i < N; i++)	{
				x[i] = complex<double>(real(x[i]) / ((double)N), imag(x[i]) / ((double)N));
			}
			return x;
		}

		//Вычисление поворачивающего модуля e^(-i*2*PI*k/N)
	private:
			static complex<double> w(int k, int N) {
				if (k % N == 0) return 1;
				double arg = -2 * PI * k / N;
				return complex<double>(cos(arg), sin(arg));
			}

			static vector<complex<double>> Conjugate (vector<complex<double>> x) {
				vector<complex<double>> out_x;
				for (int i = 0; i < x.size(); i++) {
					out_x.push_back(conj(x[i]));
				}
				return out_x;
			}
		
	};
}
