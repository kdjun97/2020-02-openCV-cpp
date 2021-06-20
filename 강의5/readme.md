# Computer Vision with c++

## Project 1
- Color Image Processing
- Color Conversion

- Develop a program which edits the input image
    - Read ‘lena.png’ an image as a color image. Depending on the input of a user, your program should do the following operations
        * Negative transformation on the user input ‘n’
        * Gamma transformation with the value of gamma as 2.5 on the user input ‘g’
        * Histogram equalization on the user input ‘h’
        * Reset the image on the user input ‘r’

- Develop a program which edits the input image
    - Read ‘colorful.jpg’ an image as a color image. Depending on the input of a user, your program should do the following operations
        * Color slicing on the user input ‘s’
            * Hue value: 9<hue<23
        * Color conversion on the user input ‘c’
            * Increase Hue value by 50
            * For hue values bigger than 129, subtract 129 instead
        * Reset the image on the user input ‘r’

여기서 Increase 부분에서 원본 소스에 166 Lines를 보면 increase 후 조건문을 비교하는데, 순서가 반대여야 함.  

- Develop a program which edits the input image
    - Read ‘balancing.jpg’ an image as a color image. Depending on the input of a user, your program should do the following operations
    * Average filtering on the user input ‘a’
        * Use “blur” function with mask size as 9X9
    * White balancing by using gray world assumption on the user input ‘w’
    * Reset the image on the user input ‘r’

- Your program should display three windows
    - 'lena', 'colorful', 'balancing'
    - Depending on the input of a user, contents in a window should be changed
    - For color conversion, use CV_BGR2HSV and CV_HSV2BGR
    - Use waitKey for user interaction
        - waitKey returns the code of the pressed key or 1 if no key was pressed before the specified time had elapsed

# Result

![Result](/강의5/result.gif)