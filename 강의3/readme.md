# Computer Vision with c++

## Spatial Filtering

- Read an image "lena.png"
    - Perform Average filtering on the left-half of the image
        - Set the mask size as (7,7)
        - blur(in, out, Size(val1, val2))
            - Blurs an image using the normalized box filter
            - in: input image, out: output image, Size(val1, val2): blurring kernel size

- Read an image "moon.png"
    - Perform sharpening on the right half of the image
        - Perform sharpening using second derivative
        - Laplacian(in, out, CV_16S);
            - calculates the Laplacian of an image
            - in: input, out: output, CV_16S: desire depth of output

- Read an image “saltnpepper.png”
    - Perform median filtering on the image
        - Set aperture size as 9
        - medianBlur (in, out, val);
            - Blurs an image using the median filter
            - in: src , out: dst , val : aperture size(must be odd and greater than 1)

- Display 6 windows
    * The name of window should be
        * "lena"
        * "lena_filtered"
        * "moon"
        * "moon_filtered"
        * "saltnpepper"
        * "saltnpepper_filtered"

# Result

![Result](/강의3/result.PNG)