/*!
* @file       image_deal.h
* @brief      ���ڵ����õĺ�����ͷ�ļ�
* @details
* @author     lvؼfox
* @version    v1.0
* @date       2019-2-26
* @to do
*
*/
# ifndef _IMAGE_DEAL_H_
# define _IMAGE_DEAL_H_


#define Deal_White  1
#define Deal_Black  0
#define ThreasHold 100//40

void MedianFilter(void);
void find_light();
void mid_findlight();
void calculate_distance();
void find_loop();
void Reset();
#endif