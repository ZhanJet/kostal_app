//Developed by Xiankun Xu: xiankunxu@gmail.com

#include <iostream>
#include <fstream>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <typeinfo>
#include <vector>
#include <stdexcept>
using namespace std;
//This is a handler to implement the FFT and IFFT algorithms on DFT and IDFT
//Definition of DFT: X(k)=sum_{n=0}^{N-1} {x(n)*W^{nk}}  with W=exp(-i 2pi/N)
//Definition of IDFT: x(n)=(1/N)sum_{k=0}^{N-1} {X(k)*W^{-nk}}  with W=exp(-i 2pi/N)

#ifndef DFT_H
#define DFT_H
class DFT
{
private:
	const double pi;
public:
	DFT():pi(atan(1)*4){}
	template <typename iter_data_type, typename iter_complexdouble_type>
	void doDFT(iter_data_type x_beg, std::size_t N_data, iter_complexdouble_type X_beg);
		//DFT operation from x -> X
		//x_beg and X_beg are iterators (pointer, iterator of STL vector or deque) point to two contiguous containers each with N data
		//take DFT on data range denoted by [x_beg, x_beg+N_data).
		//x_beg can point to data type as int, double, and complex<double>
		//The output data will be written to a range in [X_beg, X_beg+N_data)
		//X_beg must point to complex<double> type (since the DFT output about int, double, complex<double> are all in type of complex<double>)
		//Must make sure the X_beg points to a continuous data set which contains at least N_data complex<double>

	template <typename iter_data_type, typename iter_complexdouble_type>
	void doIDFT(iter_data_type X_beg, size_t N_data, iter_complexdouble_type x_beg);
		//DFT operation from X -> x
		//x_beg and X_beg are iterators (pointer, iterator of STL vector or deque) point to two contiguous containers each with N data
		//take IDFT on data range denoted by [X_beg, X_beg+N_data).
		//X_beg can point to data type as int, double, and complex<double>
		//The output data will be written to a range in [x_beg, x_beg+N_data)
		//x_beg must point to complex<double> type (since the IDFT output about int, double, complex<double> are all in type of complex<double>)
		//Must make sure the x_beg points to a continuous data set which contains at least N_data complex<double>
};
template <typename iter_data_type, typename iter_complexdouble_type>
void DFT::doDFT(iter_data_type x_beg, std::size_t N, iter_complexdouble_type X_beg)
{
	complex<double>	z(0,-2*pi/N);
	complex<double> W(exp(z)),Wk(complex<double>(1,0));

	for(iter_complexdouble_type it_X=X_beg; it_X!=X_beg+N; ++it_X)
	{
		complex<double>	Wkl(complex<double>(1,0));
		*it_X=0;

		for(iter_data_type it_x=x_beg; it_x!=x_beg+N; ++it_x)
		{
			*it_X+=(*it_x)*Wkl;
			Wkl*=Wk;
		}
		Wk*=W;
	}
}

template <typename iter_data_type, typename iter_complexdouble_type>
void DFT::doIDFT(iter_data_type X_beg, size_t N, iter_complexdouble_type x_beg)
{
	complex<double>	z(0,2*pi/N);
	complex<double> W(exp(z)),Wk(complex<double>(1,0));
	const double onedN(1.0/N);

	vector<complex<double> > xc(N);

	for(vector<complex<double> >::iterator it_x=xc.begin(); it_x!=xc.begin()+N; ++it_x)
	{
		complex<double>	Wkl(complex<double>(1,0));
		*it_x=0;

		for(iter_data_type it_X=X_beg; it_X!=X_beg+N; ++it_X)
		{
			*it_x+=(*it_X)*Wkl;
			Wkl*=Wk;
		}
		(*it_x)*=onedN;
		Wk*=W;
	}

	iter_complexdouble_type iter = x_beg;
	for (int i=0; i<N; i++) {
		(*iter)=xc[i].real();
		iter++;
	}
}
#endif //DFT_H


#ifndef FFT_handler_H
#define FFT_handler_H
class FFT_handler
{
private:
	DFT		dft_object;

public:
	template <typename iter_data_type, typename iter_complexdouble_type>
	void dft(iter_data_type x_beg, std::size_t N, iter_complexdouble_type X_beg)
	{
		dft_object.doDFT(x_beg, N, X_beg);
	}

	template <typename iter_data_type, typename iter_complexdouble_type>
	void idft(iter_data_type X_beg, size_t N, iter_complexdouble_type x_beg)
	{
		dft_object.doIDFT(X_beg, N, x_beg);
	}

	template <typename iter_data_type>
	void cutoff(iter_data_type x_beg, std::size_t N, double Ts, double cut_fc_Hz);
};

template <typename iter_data_type>
void FFT_handler::cutoff(iter_data_type x_beg, std::size_t N, double Ts, double fc)
{
	vector<complex<double> > X(N);
	dft(x_beg, N, X.begin());

	for (int i=0; i<N; i++) {
		double fi = (double)(i/N)/Ts;
		if (fi > fc && fi < (1/Ts - fc)) {
			X[i] = 0.0;
		}
	}

	idft(X.begin(), N, x_beg);
}

#endif // FFT_handler_H