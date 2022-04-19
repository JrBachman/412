__kernel void gray_conversion(
						 __global int* input,
						 __global int* output,
                         const unsigned int count)
{
    int i = get_global_id(0);
    int scount = (int) count;
    if(i < scount)
    {
		// extract r, g, b components from input[i]
		unsigned char red = (input[i] & 0x000000FF);
		unsigned char green = (input[i] & 0x0000FF00)>>8;
		unsigned char blue = (input[i] & 0x00FF0000)>>16;

		// compute gray = max(r, g, b)
		unsigned char gray;
		if(red>green && red>blue)
			gray=red;
		else if(green>red && green>blue)
			gray=green;
		else
			gray=blue;

		// produce output[i] = g g g 255
		output[i] = gray<<16 | gray<<8 | gray | 0xFF000000;

    }
}

