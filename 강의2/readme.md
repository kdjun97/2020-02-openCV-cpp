# Computer Vision with c++

## Intensity Transformation

- Read an image "lena.png"
- Perform the following operations
    - Negative transformation (Result mat: negative_img)
    - Log transformation (Result mat: log_img)
        * use log(mat a) function to perform log operation
        * To use log function, pixel type of input should be floating point
        * Also use normalize(img, img, 0, 255, NORM_MINMAX)
            - normalize img to (0~255)
- Gamma transformation with gamma as 0.5 (Result mat gamma_img)
    - Make sure you normalize pixel values from 0 to 1.0

# Result

![Result](/강의2/result.PNG)