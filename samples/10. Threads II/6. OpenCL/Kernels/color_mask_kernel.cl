__kernel void color_mask(
						 __global int* input,
						 __global int* output,
                         const unsigned int count)
{

    int i = get_global_id(0);
    int scount = (int) count;
    if(i < scount) {
		//	kill green and blue channels, preserve red and alpha
		//				         aabbggrr
        output[i] = input[i] & 0xFF0000FF;
    }
}

