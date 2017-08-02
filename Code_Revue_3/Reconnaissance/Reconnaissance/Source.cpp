#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

//int main()
//{
//	using namespace std;
//
//	//const int gros[] = { 0, 1, 2,
//	//					 3, 4, 5,
//	//					 6, 7, 8 };
//	//const int gros[] = { 0, 1, 2, 3,
//	//					 4, 5, 6, 7,
//	//					 8, 9, 10, 11,
//	//					 12, 13, 14, 15 };
//	//const int petit[] = { 4, 5,
//	//					  7, 8 };
//	//const int petit[] = { 3, 4,
//	//					  6, 7 };
//
//	const int gros[] = { 0, 0, 0,
//						 0, 1, 0,
//						 0, 0, 0 };
//	const int petit[] = { 1, 0,
//						  0, 0 };
//
//	const int gros_WIDTH = 3;
//	const int gros_HEIGHT = 3;
//
//	const int petit_WIDTH = 2;
//	const int petit_HEIGHT = 2;
//
//	const int corr_WIDTH = (gros_WIDTH - petit_WIDTH + 1);
//	const int corr_HEIGHT = (gros_HEIGHT - petit_HEIGHT + 1);
//
//	const int nbre_data_corr = corr_WIDTH * corr_HEIGHT;
//
//	int corr[nbre_data_corr];
//
//
//	int width = 0, width2 = 0, val = 0;
//	int index_gros[nbre_data_corr];
//	for (int i = 0; i < corr_HEIGHT; i++)
//	{
//		for (int j = 0; j < corr_WIDTH; j++)
//		{
//			width2 = width;
//			val = 0;
//			for (int k = 0; k < petit_HEIGHT; k++)
//			{
//				
//				for (int m = 0; m < petit_WIDTH; m++)
//				{
//					//cout << gros[width2] << endl;
//					val += gros[width2] * petit[k * petit_WIDTH + m];
//					width2++;
//
//				}
//				width2 += corr_WIDTH - 1;
//			}
//			corr[i * corr_WIDTH + j] = val;
//			cout << val << endl;
//			//cout << endl;
//			width++;
//		}
//		width += petit_WIDTH - 1;
//	}
//}