/*******************************************************************************
 * METHOD:
 * 1.string segy_file_name = "test.sgy";(include the path of the segy file)
 * 2.SegyReader segy(segy_file_name);
 * 3.you can use all the feature in class public field
 * 
 *******************************************************************************
 * Author:  Wang Yaxing,  China University of Geosciences ,Beijing, 2019-01-08
 ******************************************************************************/

#ifndef _SEGYREADER_H_
#define _SEGYREADER_H_
#include<iostream>
#include<string>
#include<fstream>

/**
* \brief the size of each part in a segy file
*/
const int TEXT_HEADER_SIZE = 3200;
const int BINARY_HEADER_SIZE = 400;
const int TRACE_HEADER_SIZE = 240;

/**
* \brief some type alias
*/
typedef unsigned char uint_8;
typedef unsigned short uint_16;
typedef unsigned long uint_32;
typedef char int_8;
typedef short int_16;
typedef long int_32;

/**
 * \brief 40*80=3200 ebcdic header
 */
struct Ehdr
{
	uint_8 text_header[TEXT_HEADER_SIZE];
};

/**
 * \brief binary header 400
 */
struct Bhdr
{
	uint_32 job_id_number;							// 3201-3204	4			Job identification number					��ҵ��ʶ��
	uint_32 line_number;							// 3205-3208	4			Line number									���ߺţ�ÿ��ֻ�ܼ�һ���ߣ�
	uint_32 reel_number;							// 3209-3212	4			Reel number									���
	uint_16 traces_per_record;						// 3213-3214	2			data traces per record						ÿ����¼�����ݵ����������յ��ͳ������
	uint_16 aux_traces_per_record;					// 3215-3216	2			aux traces per record						ÿ����¼�ĸ�������
	uint_16 sample_data_interval_ms;				// 3217-3218	2			sample interval(microseconds) for reel		�������(��s)
	uint_16 original_data_interval_ms;				// 3219-3220	2			sample interval(microseconds) for field		��΢������ԭʼҰ���¼�Ĳ������
	uint_16 samples_per_trace;						// 3221-3222	2			number samples per data trace for reel		������ÿ�����ݵ��Ĳ�������
	uint_16 original_samples_per_trace;				// 3223-3224	2			number samples per data trace for field		Ұ��ԭʼ��¼ÿ�����ݵ��Ĳ�������
	uint_16 data_sample_format_code;				// 3225-3226	2			data sample format code						���ݲ�����ʽ�룺1-���㣨4�ֽڣ���2-���㣨4�ֽڣ���3-���㣨2�ֽڣ���4-����������Ķ��㣨4�ֽڣ�,������ÿ��������ͬ�����ֽ���
	uint_16 CDP_fold;								// 3227-3228	2			CDP fold									CDP���Ǵ���
	uint_16 trace_sorting_code;						// 3229-3230	2			trace sorting code							����ѡ�룺1-ԭʼ��¼���ڼ�����2-CDP������3-���θ������棻4-ˮƽ����
	uint_16 vertical_sum_code;						// 3231-3232	2			vertical sum code							��ֱ���Ӵ��룺1-δ���ӣ�2-���ε��ӣ�������n-n�ε���
	uint_16 sweep_frequency_start_hz;				// 3233-3234	2			sweep frequently at start					��ʼɨ��Ƶ�ʣ�Hz��
	uint_16 sweep_frequency_end_hz;					// 3235-3236	2			sweep frequently at end						����ɨ��Ƶ�ʣ�Hz��
	uint_16 sweep_length_ms;						// 3237-3238	2			sweep lenth (milliseconds)_					ɨ�賤�ȣ�ms��
	uint_16 sweep_type_code;						// 3239-3240	2			sweep type code								ɨ�����ʹ��룺1-���ԣ�2-�����ߣ�3-ָ����4-����
	uint_16 trace_number_of_sweep_channel;			// 3241-3242	2			trace number of sweep chanel				ɨ��ͨ���ĵ���
	uint_16 sweep_trace_taper_length_start_ms;		// 3243-3244	2			sweep trace taper length at start (ms)		�����б�£�Ϊ��ʼʱ��ɨ��б�³��ȣ�ms����б�������ʱ���㣩
	uint_16 sweep_trace_taper_length_end_ms;		// 3245-3246	2			sweep trace taper length at end (ms)		����ʱ��б�³��ȣ�б�����Ϊɨ�賤�ȼ�β����б�³���
	uint_16 taper_type_code;						// 3247-3248	2			taper type									б�����ͣ�1-���Σ�2-COS^2��3-����
	uint_16 correlated_data_traces_flag;			// 3249-3250	2			corellated data traces						������ݵ����룺1-δ��أ�2-���
	uint_16 binary_gain_recovered_flag;				// 3251-3252	2			binary gain recoverd						����������ָ���1-�ָ���2-δ�ָ�
	uint_16 amplitude_recovery_method_code;			// 3253-3254	2			amplitude recovery method					����ָ���ʽ��1-δ�ָ���2-������ɢ��3-AGC��4-����
	uint_16 measurement_system;						// 3255-3256	2			measuriment system (1-m/2-feet)				���ȵ�λ��1-�ף�2-Ӣ��
	uint_16 impulse_signal_polarity;				// 3257-3258	2			impulse signal								�����źż��ԣ�1-�Ŵ��ϵĸ�ֵΪѹ�����ӻ�첨��������2-�Ŵ��ϵ���ֵΪѹ�����ӻ�첨������
	uint_16 vibratory_polarity_code;				// 3259-3260	2			vibratory polarity code						�ɿ���Դ���Դ���
	uint_16 a_undefined[120]; 					    // 3259-3260	240			δ��ֵ
	uint_16 segy_format_revision_num;				// 3259-3260	2			SEG Y��ʽ�޶����                            ��ֵ��ʾ���1975���׼�ġ���ͳ��SEG Y
	uint_16 fixed_length_flag;					    // 3259-3260	2			�̶����ȵ���־						        1��ʾSEG Y�ļ������е�ȷ��������ͬ�Ĳ�������Ͳ�������,0��ʾ�ļ��еĵ������ܱ仯
	uint_16 extended_textual_header_num;			// 3259-3260	2			3200�ֽ���չԭ���ļ�ͷ��¼�ڶ�����ͷ��			��ֵ��ʾ���1975���׼�ġ���ͳ��SEGY,û����չԭ���ļ�ͷ��¼
	uint_16 b_undefined[47];					    // 3259-3260	94			δ��ֵ
};

struct Thdr
{
	uint_32 trace_sequence_number_within_line;								// 1-4     4   һ�����ߵĵ���ţ����һ�����ж�������������������ӵ�
	uint_32 trace_sequence_number_within_reel;								// 5-8     4   ������ĵ���š�ÿ��������1��ʼ
	uint_32 original_field_record_number;									// 9-12    4*  ԭʼҰ���¼��(�ں�)
	uint_32 trace_sequence_number_within_original_field_record;				// 13-16   4   ԭʼҰ���¼�ĵ���
	uint_32 energy_source_point_number;										// 17-20   4   ��Դ���
	uint_32 cdp_ensemble_number;											// 21-24   4*  CDP��(��CMP, CRP)
	uint_32 trace_sequence_number_within_cdp_ensemble;						// 25-28   4*  ��CDP�����еĵ��š�ÿ����������ʼ����Ϊ1

	uint_16 trace_identification_code;										// 29-30   2   ��ʶ��
	uint_16 number_of_vertically_summed_traces_yielding_this_trace;			// 31-32   2   ��ֱ�����֣������ɶ��ٵ����Ӷ���
	uint_16 number_of_horizontally_stacked_traced_yielding_this_trace;		// 33-34   2   CDP���Ǵ���
	uint_16 data_use;														// 35-36   2   �������ʣ�1=�ɹ���2=ʵ��

	uint_32 distance_from_source_point_to_receiver_group;					// 37-40   4*  �ڵ㵽�첨����루��ֵ��ʾ����߷��ڷ����෴��
	int_32 receiver_group_elevation;										// 41-44   4   �첨���ĸ߶ȣ��ں�ƽ������Ϊ��������Ϊ��
	int_32 surface_elevation_at_source;										// 45-48   4   ��Դ��ˮƽ�߶�
	int_32 source_depth_below_surface;										// 49-52   4   ��Դ��ˮƽ�µ���ȣ���һ��������
	int_32 datum_elevation_at_receiver_group;								// 53-56   4   �첨���ĺ���
	int_32 datum_elevation_at_source;										// 57-60   4   ��Դ�ĺ���
	int_32 water_depth_at_source;											// 61-64   4   ��Դ��ˮ��
	int_32 water_depth_at_receiver_group;									// 65-68   4   �첨����ˮ��

	int_16 scalar_for_elevations_and_depths;								// 69-70   2   �������ӣ����ֽ�41~68�ĸ̺߳����ʹ�ñ������Ӻ������ֵ����������Ϊ1����10����100����1000���10000�����Ϊ��������
	int_16 scalar_for_coordinates;											// 71-72   2   �������ӣ����ֽ�73��88��199��206�ĸ̺߳����ʹ�ñ������Ӻ������ֵ����������Ϊ1����10����100����1000���10000�����Ϊ�������ϱ������ӣ����Ϊ�������Ա������ӡ�

	int_32 x_source_coordinate;												// 73-76   4   ��Դ������-X
	int_32 y_source_coordinate;												// 77-80   4   ��Դ������-Y
	int_32 x_receiver_group_coordinate;										// 81-84   4   �첨��������-X
	int_32 y_receiver_group_coordinate;										// 85-88   4   �첨��������-Y

	uint_16 coordinate_units;												// 89-90   2   ���굥λ��1=���ȣ��׻�Ӣ�ߣ� 2=���ȵ���
	uint_16 weathering_velocity;											// 91-92   2   �绯���ٶ�
	uint_16 subweathering_velocity;											// 93-94   2   ���ٲ��ٶ�
	uint_16 uphole_time_at_source;											// 95-96   2   ����Դ���ľ���ʱ��
	uint_16 uphole_time_at_group;											// 97-98   2   �ڼ첨�㴦�ľ���ʱ��
	uint_16 source_static_correction;										// 99-100  2   �ڵ㾲У��
	uint_16 group_static_correction;										// 101-102 2   �첨����У��
	uint_16 total_static_applied;											// 103-104 2   Ӧ�õ��ܾ�У���������δʹ�ã���Ϊ�㣩
	uint_16 lag_time_a;														// 105-106 2   �ӳ�ʱ��A��ms������¼�ĵ�һ�������ʱ���ź�֮���ʱ�䡣240�ֽڵ���ʶͷ��ʱ���ź�֮���ʱ�䡣���Ϊ����ʱ���źų����ڵ���ʶͷ����֮�����Ϊ����ʱ���źų����ڵ���ʶͷ����֮ǰ
	uint_16 lag_time_b;														// 107-108 2   �ӳ�ʱ��B��ms����ʱ���źź���Դ��ʱ��֮���ʱ��
	uint_16 delay_according_time;											// 109-110 2   �ӳټ�¼ʱ�䣨ms������Դ��ʱ��Ϳ�ʼ��¼����ʱ��֮���ʱ�䣨��ˮʱʹ�ã�
	uint_16 mute_time_start;												// 111-112 2   ��ʼ�г�ʱ��  //brute_time_start
	uint_16 mute_time_end;													// 113-114 2   �����г�ʱ��
	uint_16 samples_in_this_trace;											// 115-116 2*  ������������
	uint_16 sample_intervall;												// 117-118 2*  �������
	uint_16 gain_type_instruments;											// 119-120 2   Ұ���������������ͣ�1=�̶� 2=������ 3=������ 4=��ѡ������
	uint_16 igc;															// 121-122 2   �������泣��
	uint_16 igi;															// 123-124 2   �������ں����������
	uint_16 corr;															// 125-126 2   ����ԣ�1=û�� 2=��
	uint_16 sfs;															// 127-128 2   �ڿ�ʼʱ��ɨ��Ƶ��
	uint_16 sfe;															// 129-130 2   �ڽ���ʱ��ɨ��Ƶ��
	uint_16 slen;															// 131-132 2   ɨ�賤���Ժ���Ϊ��λ
	uint_16 styp;															// 133-134 2   ɨ������  1 ���� 2�������� 3ָ����
	uint_16 stas;															// 135-136 2   �ڿ�ʼʱ��ɨ�����
	uint_16 stae;															// 137-138 2   �ڽ���ʱ��ɨ�����
	uint_16 tatyp;															// 139-140 2   ɨ������  1 ���� 2�������� 3ָ����
	uint_16 afilf;															// 141-142 2   Alias�˲���Ƶ�ʣ����ʹ����
	uint_16 afils;															// 143-144 2   Alias�˲���б��
	uint_16 nofilf;															// 145-146 2   �ݲ��˲���Ƶ��
	uint_16 nofils;															// 147-148 2   �ݲ��˲���б��
	uint_16 lcf;															// 149-150 2   ����Ƶ
	uint_16 hcf;															// 151-152 2   ����Ƶ
	uint_16 lcs;															// 153-154 2   ����б
	uint_16 hcs;															// 155-156 2   ����б��
	uint_16 year;															// 157-158 2   ��¼����
	uint_16 day;															// 159-160 2   ����ĵڼ���
	uint_16 hour;															// 161-162 2   ����ĵڼ�Сʱ
	uint_16 minute;															// 163-164 2   ���Сʱ�ĵڼ���
	uint_16 sec;															// 165-166 2   ��ֵĵڼ���
	uint_16 timbas;															// 167-168 2   ʱ���� I = local.	2 = GMT. 3 = other.
	uint_16 trwf;															// 169-170 2   trace weighting factor
	uint_16 grnors;															// 171-172 2   geophone group number of roll switch position one
	uint_16 grnofr;															// 173-174 2   geophone group number of trace one within original field record
	uint_16 grnlof;															// 175-176 2   geophone group number of last trace within original field record
	uint_16 gaps;															// 177-178 2   gap size (total number of groups dropped)
	uint_16 otrav;															// 179-180 2   overtravel taper code: 1 = down (or behind) 2 = up (or ahead)
																			/*-----------------------------------2018��4��11��15:45:23----------------------------*/

	int_32 x_cdp_coordinate;                                                // 181-184 4   �õ��ĵ�����CDP��λ��X����
	int_32 y_cdp_coordinate;                                                // 185-188 4   �õ��ĵ�����CDP��λ��y����
	uint_32 num_in_line;                                                    // 189-192 4   ��ά�������ݣ�In-line��
	uint_32 num_cross_line;                                                 // 193-196 4   ��ά�������ݣ�Cross-line��
	uint_32 num_shot_point;													// 197-200 4   �ڵ��

	uint_16 scalar_shot_point;                                              // 201-202 2   Ӧ���ڵ�ͷ��197-200�ֽ����ڵ�ŵ����ӣ��Եõ�ʵ����ֵ����Ϊ��������������������Ϊ��������������������Ϊ�㣻�ڵ�Ų�������������
	uint_16 unit_trace_value;                                               // 203-204 2   ��ֵ������λ

	uint_32 constant_transuction_int;                                       // 205-210 6   ת������-�ñ������ڽ����ݵ�����ת����ת����λ

	uint_16 constant_transuction_pow;
	uint_16 unit_transduction;                                              // 211-212 2   ת����λ-���Ե�ͷ205-210�ֽ��е�ת������������ݵ�����������λ��
	uint_16 identifier_trace;                                               // 213-214 2   �豸/����ʶ-�����ݵ������ĵ�λ�Ż��豸��
	uint_16 scalar_trace;                                                   // 215-216 2   �ڵ�ͷ95-114�ֽڸ�����������ʱ�������
	uint_16 source_orientation;                                             // 217-218 2   �������ͻ�����Դ�ķ�λ
	uint_16 source_energy_direction[3];                                     // 219-224 6   �����Դ��λ����Դ��������D�D����λ�����ڵ�ͷ217-218�ֽڶ��塣���������Զ������ȱ��루��347.8������3478��
	uint_16 source_measurement[3];                                          // 225-230 6   ��������������ԴЧӦ
	uint_16 unit_source_measurement;                                        // 231-232 2   ��Դ������λ�D�D������Դ��������ͷ225-230�ֽڵĵ�λ��-1=����0=δ֪1=������J��2=ǧ�ߣ�kW��3=��˹����Pa��4=�ͣ�Bar��4=�ͣ��ף�Bar-m��5=ţ�٣�N��6=ǧ�ˣ�kg��

	uint_8 un_define[8];                                                    // 233-240 8   δ��ֵ�D�DΪ��ѡ��ϢԤ��

};

/**
 * \brief all function in this SegyReader class
 */
class SegyReader
{
public:
	SegyReader();
	SegyReader(const std::string& segy_file_name);
	~SegyReader();

	int getSampleSum(void) const;
	int getTraceSum(void) const;
	float getSpecificData(const int trace_num, const int sample_num);//one point
	void readTraceHeader(const int trace_num);
	void outputTD2Txt(const std::string& output_file_name, const int single_trace_num);
	void outputTD2Txt(const std::string& output_file_name, const int trace_num_beg, const int trace_num_end);
	void outputTD2Binary(const std::string& output_file_name, const int single_trace_num);
	void outputTD2Binary(const std::string& output_file_name, const int trace_num_beg, const int trace_num_end);

private:
	std::string file_name;
	std::ifstream segy_input;
	FILE *segy_file{};//for reading data
	Bhdr *bhdr{};
	Ehdr *ehdr{};
	Thdr *thdr{};
	
	
	int sample_sum{};
	int trace_sum{};
	int data_type_code{};
	enum DataType { IBM = 1, IEEE = 5, PC = 6 };

	int getDataType(void);
	int readSampleSum(void);
	int readTraceSum(void);
	bool convertBinary(void *data, int size);
	void readBinaryHeader(void);
	void readTextHeader(void);
	void swapByte2(uint_16 *data);
	void swapByte4(uint_32 *data);
	void swapBinaryHeader(Bhdr *bhdr);
	void swapTraceHeader(Thdr *thdr);
	void ebcdic2Ascii(Ehdr *tmp_ehdr);
};
#endif //!_SEGYREADER_H_