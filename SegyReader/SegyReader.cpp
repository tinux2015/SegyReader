#include "SegyReader.h"
#include <iomanip>
using namespace std;
const unsigned int data_bytes[] = { 4, 4, 2, 4, 4, 1 };
//Ibm×ª»¯ÎªIEEE
void ibm2Ieee(float* input, int swap)
{
	uint_8  *cbuf, expp, tem, sign;
	uint_32  *umantis, expll;
	long *mantis;
	int  shift;

	cbuf = (uint_8*)&input[0];        /* assign address of input to char array */
	umantis = (uint_32*)&input[0];     /* two differnt points to the same spot  */
	mantis = (long*)&input[0];     /* signned & unsigned                    */

	if (swap)
	{
		/* now byte reverce for PC use if swap true */
		tem = cbuf[0]; cbuf[0] = cbuf[3]; cbuf[3] = tem;
		tem = cbuf[2]; cbuf[2] = cbuf[1]; cbuf[1] = tem;
	}

	/* start extraction information from number */

	expp = *mantis >> 24;     /* get expo fro upper byte      */
	*mantis = (*mantis) << 8; /* shift off upper byte         */
	shift = 1;              /* set a counter to 1           */
	while (*mantis>0 && shift<23) /* start of shifting data*/
	{
		*mantis = *mantis << 1;
		shift++;
	} /* shift until a 1 in msb */

	*mantis = *mantis << 1; /* need one more shift to get implied one bit */
	sign = expp & 0x80;   /* set sign to msb of exponent            */
	expp = expp & 0x7F;   /* kill sign bit                          */

	if (expp != 0)        /* don't do anymore if zero exponent       */
	{
		expp = expp - 64;   /* compute what shift was (old exponent)*/
		*umantis = *umantis >> 9; /* MOST IMPORTANT an UNSIGNED shift back down */
		expll = 0x7F + (expp * 4 - shift); /* add in excess 172 */

		/* now mantissa is correctly aligned, now create the other two pairs */
		/* needed the extended sign word and the exponent word               */

		expll = expll << 23;        /* shift exponent up */

		/* combine them into a floating point IEEE format !     */

		if (sign) *umantis = expll | *mantis | 0x80000000;
		else     *umantis = expll | *mantis; /* set or don't set sign bit */
	}
}
/**
 * \brief change two bytes(not the bit) 1 2 to 2 1
 * \param data 
 */
void SegyReader::swapByte2(uint_16 *data)
{
	uint_16 *pdata = data;
	(*pdata) = ((*pdata) << 8) | ((*pdata) >> 8);
}

/**
 * \brief change four bytes(not the bit) 1 2 3 4 to 4 3 2 1
 * \param data 
 */
void SegyReader::swapByte4(uint_32 *data)
{
	uint_32 *pdata = data;
	(*pdata) = ((*pdata) << 8 & 0xFF00FF00) | ((*pdata) >> 8 & 0x00FF00FF);
	(*pdata) = ((*pdata) >> 16) | ((*pdata) << 16);
}

/**
 * \brief convert 2 or 4 byte for the header or data
 * \param data 
 * \param size 
 * \return 
 */
bool SegyReader::convertBinary(void* data, int size)
{
	switch ((size))
	{
	case 2:
		swapByte2(static_cast<uint_16 *>(data));
		return true;
	case 4:
		swapByte4(static_cast<uint_32 *>(data));
		return true;
	default:
		return false;
	}
}

void SegyReader::swapBinaryHeader(Bhdr* bhdr)
{
	convertBinary(&((*bhdr).job_id_number), 4);
	convertBinary(&((*bhdr).line_number), 4);
	convertBinary(&((*bhdr).reel_number), 4);

	convertBinary(&((*bhdr).traces_per_record), 2);
	convertBinary(&((*bhdr).aux_traces_per_record), 2);
	convertBinary(&((*bhdr).sample_data_interval_ms), 2);
	convertBinary(&((*bhdr).original_data_interval_ms), 2);
	convertBinary(&((*bhdr).samples_per_trace), 2);
	convertBinary(&((*bhdr).original_samples_per_trace), 2);
	convertBinary(&((*bhdr).data_sample_format_code), 2);
	convertBinary(&((*bhdr).CDP_fold), 2);
	convertBinary(&((*bhdr).trace_sorting_code), 2);
	convertBinary(&((*bhdr).vertical_sum_code), 2);
	convertBinary(&((*bhdr).sweep_frequency_start_hz), 2);
	convertBinary(&((*bhdr).sweep_frequency_end_hz), 2);
	convertBinary(&((*bhdr).sweep_length_ms), 2);
	convertBinary(&((*bhdr).sweep_type_code), 2);
	convertBinary(&((*bhdr).trace_number_of_sweep_channel), 2);
	convertBinary(&((*bhdr).sweep_trace_taper_length_start_ms), 2);
	convertBinary(&((*bhdr).sweep_trace_taper_length_end_ms), 2);
	convertBinary(&((*bhdr).taper_type_code), 2);
	convertBinary(&((*bhdr).correlated_data_traces_flag), 2);
	convertBinary(&((*bhdr).binary_gain_recovered_flag), 2);
	convertBinary(&((*bhdr).amplitude_recovery_method_code), 2);
	convertBinary(&((*bhdr).measurement_system), 2);
	convertBinary(&((*bhdr).impulse_signal_polarity), 2);
	convertBinary(&((*bhdr).vibratory_polarity_code), 2);
}

/**
 * \brief swap the segy file trace header 
 * \param thdr 
 */
void SegyReader::swapTraceHeader(Thdr * thdr)
{
	convertBinary(&((*thdr).trace_sequence_number_within_line), 4);
	convertBinary(&((*thdr).trace_sequence_number_within_reel), 4);
	convertBinary(&((*thdr).original_field_record_number), 4);
	convertBinary(&((*thdr).trace_sequence_number_within_original_field_record), 4);
	convertBinary(&((*thdr).energy_source_point_number), 4);
	convertBinary(&((*thdr).cdp_ensemble_number), 4);
	convertBinary(&((*thdr).trace_sequence_number_within_cdp_ensemble), 4);

	convertBinary(&((*thdr).trace_identification_code), 2);
	convertBinary(&((*thdr).number_of_vertically_summed_traces_yielding_this_trace), 2);
	convertBinary(&((*thdr).number_of_horizontally_stacked_traced_yielding_this_trace), 2);
	convertBinary(&((*thdr).data_use), 2);

	convertBinary(&((*thdr).distance_from_source_point_to_receiver_group), 4);
	convertBinary(&((*thdr).receiver_group_elevation), 4);
	convertBinary(&((*thdr).surface_elevation_at_source), 4);
	convertBinary(&((*thdr).source_depth_below_surface), 4);
	convertBinary(&((*thdr).datum_elevation_at_receiver_group), 4);
	convertBinary(&((*thdr).datum_elevation_at_source), 4);
	convertBinary(&((*thdr).water_depth_at_source), 4);
	convertBinary(&((*thdr).water_depth_at_receiver_group), 4);

	convertBinary(&((*thdr).scalar_for_elevations_and_depths), 2);
	convertBinary(&((*thdr).scalar_for_coordinates), 2);

	convertBinary(&((*thdr).x_source_coordinate), 4);
	convertBinary(&((*thdr).y_source_coordinate), 4);
	convertBinary(&((*thdr).x_receiver_group_coordinate), 4);
	convertBinary(&((*thdr).y_receiver_group_coordinate), 4);

	convertBinary(&((*thdr).coordinate_units), 2);
	convertBinary(&((*thdr).weathering_velocity), 2);
	convertBinary(&((*thdr).subweathering_velocity), 2);
	convertBinary(&((*thdr).uphole_time_at_source), 2);
	convertBinary(&((*thdr).uphole_time_at_group), 2);
	convertBinary(&((*thdr).source_static_correction), 2);
	convertBinary(&((*thdr).group_static_correction), 2);
	convertBinary(&((*thdr).total_static_applied), 2);
	convertBinary(&((*thdr).lag_time_a), 2);
	convertBinary(&((*thdr).lag_time_b), 2);
	convertBinary(&((*thdr).delay_according_time), 2);
	convertBinary(&((*thdr).mute_time_start), 2);
	convertBinary(&((*thdr).mute_time_end), 2);
	convertBinary(&((*thdr).samples_in_this_trace), 2);
	convertBinary(&((*thdr).sample_intervall), 2);
	convertBinary(&((*thdr).gain_type_instruments), 2);
	convertBinary(&((*thdr).igc), 2);
	convertBinary(&((*thdr).igi), 2);
	convertBinary(&((*thdr).corr), 2);
	convertBinary(&((*thdr).sfs), 2);
	convertBinary(&((*thdr).sfe), 2);
	convertBinary(&((*thdr).slen), 2);
	convertBinary(&((*thdr).styp), 2);
	convertBinary(&((*thdr).stas), 2);
	convertBinary(&((*thdr).stae), 2);
	convertBinary(&((*thdr).tatyp), 2);
	convertBinary(&((*thdr).afilf), 2);
	convertBinary(&((*thdr).afils), 2);
	convertBinary(&((*thdr).nofilf), 2);
	convertBinary(&((*thdr).nofils), 2);
	convertBinary(&((*thdr).lcf), 2);
	convertBinary(&((*thdr).hcf), 2);
	convertBinary(&((*thdr).lcs), 2);
	convertBinary(&((*thdr).hcs), 2);
	convertBinary(&((*thdr).year), 2);
	convertBinary(&((*thdr).day), 2);
	convertBinary(&((*thdr).hour), 2);
	convertBinary(&((*thdr).minute), 2);
	convertBinary(&((*thdr).sec), 2);
	convertBinary(&((*thdr).timbas), 2);
	convertBinary(&((*thdr).trwf), 2);
	convertBinary(&((*thdr).grnors), 2);
	convertBinary(&((*thdr).grnofr), 2);
	convertBinary(&((*thdr).grnlof), 2);
	convertBinary(&((*thdr).gaps), 2);
	convertBinary(&((*thdr).otrav), 2);

	convertBinary(&((*thdr).x_cdp_coordinate), 4);
	convertBinary(&((*thdr).y_cdp_coordinate), 4);
}

int SegyReader::readSampleSum()
{
	if (bhdr)
	{
		sample_sum = (*bhdr).samples_per_trace;
		return sample_sum;
	}
	else
	{
		cerr << "Wrong,no binary header file open." << endl;
		abort();
	}
}

int SegyReader::readTraceSum()
{
	if (!segy_input.is_open())
	{
		cerr << "Please open a segy file .Check the Function--getTraceNum()" << endl;
		abort();
	}
	const long offset = 0 - (sample_sum * data_bytes[data_type_code-1] + 240);
	(segy_input).seekg(offset, ios::end);
	
	const auto tmp_thdr = new Thdr;
	(segy_input).read(reinterpret_cast<char*>(tmp_thdr), TRACE_HEADER_SIZE);
	swapTraceHeader(tmp_thdr);
	trace_sum = (*tmp_thdr).trace_sequence_number_within_original_field_record;
	delete tmp_thdr;
	return trace_sum;
}

void SegyReader::ebcdic2Ascii(Ehdr* tmp_ehdr) 
{
	char ebcdic_to_ascii_table[256] =
	{
		0x00,0x01,0x02,0x03,0x00,0x09,0x00,0x7f,
		0x00,0x00,0x00,0x0b,0x0c,0x0d,0x0e,0x0f,
		0x10,0x11,0x12,0x13,0x00,0x00,0x08,0x00,
		0x18,0x19,0x00,0x00,0x1c,0x1d,0x1e,0x1f,
		0x00,0x00,0x00,0x00,0x00,0x0a,0x17,0x1b,
		0x00,0x00,0x00,0x00,0x00,0x05,0x06,0x07,
		0x00,0x00,0x16,0x00,0x00,0x00,0x00,0x04,
		0x00,0x00,0x00,0x00,0x14,0x15,0x00,0x1a,
		0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x2e,0x3c,0x28,0x2b,0x7c,
		0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x21,0x24,0x2a,0x29,0x3b,0x5e,
		0x2d,0x2f,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x2c,0x25,0x5f,0x3e,0x3f,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x60,0x3a,0x23,0x40,0x27,0x3d,0x22,
		0x00,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
		0x68,0x69,0x00,0x7b,0x00,0x00,0x00,0x00,
		0x00,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,
		0x71,0x72,0x00,0x7d,0x00,0x00,0x00,0x00,
		0x00,0x7e,0x73,0x74,0x75,0x76,0x77,0x78,
		0x79,0x7a,0x00,0x00,0x00,0x5b,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x5d,0x00,0x00,
		0x7b,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
		0x48,0x49,0x00,0x00,0x00,0x00,0x00,0x00,
		0x7d,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,
		0x51,0x52,0x00,0x00,0x00,0x00,0x00,0x00,
		0x5c,0x00,0x53,0x54,0x55,0x56,0x57,0x58,
		0x59,0x5a,0x00,0x00,0x00,0x00,0x00,0x00,
		0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
		0x38,0x39,0x00,0x00,0x00,0x00,0x00,0x00,
	};
	for (unsigned char& i : (*tmp_ehdr).text_header) //from ebcdicvonverter.cpp
	{
		i = ebcdic_to_ascii_table[i];
	}
}

/**
 * \brief default constructor
 */
SegyReader::SegyReader() = default;


/**
 * \brief overload constructor
 * \param segy_file_name 
 */
SegyReader::SegyReader(const string& segy_file_name): file_name(segy_file_name),segy_input(segy_file_name)
{
	if (segy_input.is_open())
	{
		cout << "Open success!" << endl;
	}
	fopen_s(&segy_file, segy_file_name.c_str(), "rb");
	readTextHeader();
	readBinaryHeader();
	data_type_code = getDataType();
	sample_sum = readSampleSum();
	trace_sum = readTraceSum();
}


/**
 * \brief destructor
 */
SegyReader::~SegyReader()
{
	fclose(segy_file);
	(segy_input).close();
	delete bhdr;
	bhdr = nullptr;
	delete ehdr;
	ehdr = nullptr;
	delete thdr;
	thdr = nullptr;
}

/**
 * \brief read the (trace_num)th trace header
 * \param trace_num 
 */
void SegyReader::readTraceHeader(const int trace_num)
{
	if (!thdr)
	{
		delete thdr;
		thdr = nullptr;
	}
	const auto tmp_thdr = new Thdr;
	if (trace_num<=trace_sum && trace_num>=1)
	{
		const uint_32 byte_offset = 3600 + (trace_num - 1)*(240 + data_bytes[data_type_code-1] * sample_sum);
		(segy_input).seekg(byte_offset, ios::beg);
		memset(tmp_thdr, '\0', TRACE_HEADER_SIZE);
		(segy_input).read(reinterpret_cast<char*>(tmp_thdr), TRACE_HEADER_SIZE);
		swapTraceHeader(tmp_thdr);
		thdr = tmp_thdr;
	}
	else
	{
		cerr << "Something wrong with Function -- readTraceHeader(),please check." << endl;
	}
}

/**
 * \brief get the specific data at the position of (trace_num,sample_num) from the file 
 * \param trace_num 
 * \param sample_num 
 * \return 
 */
float SegyReader::getSpecificData(const int trace_num, const int sample_num)
{
	const int tmp_bytes = data_bytes[data_type_code-1];
	if ((trace_num >= 1) &&(trace_num <= trace_sum) &&(sample_num >= 1)&&(trace_num <= trace_sum))
	{   
		float result{ 0 };
		
		const size_t byte_offset = 3600 + (trace_num - 1)*(240 + tmp_bytes * sample_sum) + 240 + (sample_num - 1) * tmp_bytes;
		switch (data_type_code)
		{
		case IBM:
			fseek(segy_file, byte_offset, SEEK_SET);
			if (0 != fread(&result, tmp_bytes, 1, segy_file))
			{
				ibm2Ieee(&result, 1);
				return result;
			}
			else
			{
				cerr << "There is something wrong in getSpecificData." << endl;
				abort();
			}
		case IEEE:
			fseek(segy_file, byte_offset, SEEK_SET);
			if (0 != fread(&result, tmp_bytes, 1, segy_file))
			{
				swapByte4(reinterpret_cast<uint_32 *>(&result));
				return result;
			}
			else
			{
				cerr << "There is something wrong in getSpecificData." << endl;
				abort();
			}
		default:
			cerr << "These function has not been finished." << endl;
			abort();
		}
	}
	else
	{
		cerr << "Trace_num or sample_num out of range.Please check." << endl;
		abort();
	}
}

/**
 * \brief output just one single trace to a txt file
 * \param output_file_name (the name of output file,no need the file extension)
 * \param single_trace_num 
 */
void SegyReader::outputTD2Txt(const string& output_file_name, const int single_trace_num)
{
	outputTD2Txt(output_file_name, single_trace_num, single_trace_num);
}

/**
 * \brief output the trace data from trace_num_beg to the trace_num_end to a txt file
 * \param output_file_name 
 * \param trace_num_beg 
 * \param trace_num_end 
 */
void SegyReader::outputTD2Txt(const string& output_file_name, const int trace_num_beg, const int trace_num_end)
{
	ofstream outfile(output_file_name+".txt", ios::out);
	if ((trace_num_beg >= 1) && (trace_num_end <= trace_sum) && (trace_num_beg <= trace_num_end))
	{
		for (int sample_num = 1; sample_num <= sample_sum; sample_num++)
		{
			for (int trace_num = trace_num_beg; trace_num <= trace_num_end; trace_num++)
			{
				const float tmp_data = getSpecificData(trace_num, sample_num);
				outfile <<tmp_data<<" ";
			}
			outfile << endl;
		}
	}

	outfile.close();
}

/**
 * \brief output just one single trace to a binary file
 * \param output_file_name 
 * \param single_trace_num 
 */
void SegyReader::outputTD2Binary(const string& output_file_name, const int single_trace_num)
{
	outputTD2Binary(output_file_name, single_trace_num, single_trace_num);
}

/**
 * \brief output the trace data from trace_num_beg to the trace_num_end to a binary file
 * \param output_file_name 
 * \param trace_num_beg 
 * \param trace_num_end 
 */
void SegyReader::outputTD2Binary(const string& output_file_name, const int trace_num_beg, const int trace_num_end)
{
	ofstream outfile(output_file_name+".dat", ios::out|ios::binary);
	if ((trace_num_beg >= 1) && (trace_num_end <= trace_sum) && (trace_num_beg <= trace_num_end))
	{
		for (int sample_num = 1; sample_num <= sample_sum; sample_num++)
		{
			for (int trace_num = trace_num_beg; trace_num <= trace_num_end; trace_num++)
			{
				const float tmp_data = getSpecificData(trace_num, sample_num);
				outfile << tmp_data << " ";
			}
			outfile << endl;
		}
	}
	cout << output_file_name <<" Output success." << endl;

	outfile.close();
}

/**
 * \brief get the sample sum
 * \return 
 */
int SegyReader::getSampleSum() const
{
	return sample_sum;
}

/**
 * \brief get the trace sum
 * \return 
 */
int SegyReader::getTraceSum() const
{
	return trace_sum;
}

/**
 * \brief get the data type(3225-3226) in the segy file
 * \return 
 */
int SegyReader::getDataType(void)
{
	if (bhdr)
	{
		 data_type_code = static_cast<int>((*bhdr).data_sample_format_code);
		 return data_type_code;
	}
	else
	{
		cerr << "Something wrong with the Function -- getDataType(void),please check."<< endl;
		abort();
	}
}

/**
 * \brief read the text header from the segy file
 */
void SegyReader::readTextHeader(void)
{
	const auto tmp_ehdr = new Ehdr;
	memset(tmp_ehdr, '\0', TEXT_HEADER_SIZE);
	(segy_input).seekg(0, ios::beg);
	(segy_input).read(reinterpret_cast<int_8 *>(tmp_ehdr), TEXT_HEADER_SIZE);
	ebcdic2Ascii(tmp_ehdr);
	ehdr = tmp_ehdr;
	(segy_input).seekg(0, ios::beg);//move the pointer to the beginning
}

/**
 * \brief read binary header from segy file
 */
void SegyReader::readBinaryHeader(void)
{
	auto tmp_bhdr = new Bhdr;
	//init
	memset(reinterpret_cast<int_8 *>(tmp_bhdr), '\0', BINARY_HEADER_SIZE);
	//move the pointer to the beginning of the binary header
	(segy_input).seekg(TEXT_HEADER_SIZE, ios::beg);
	//assignment
	(segy_input).read(reinterpret_cast<int_8 *>(tmp_bhdr), BINARY_HEADER_SIZE);
	//swap the binary header 
	swapBinaryHeader(tmp_bhdr);
	bhdr = tmp_bhdr;
	//move the pointer to the beginning
	(segy_input).seekg(0, ios::beg);
}