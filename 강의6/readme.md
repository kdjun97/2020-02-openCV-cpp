# Computer Vision with c++

# Edge Detection

- Develop a program which displays canny edges of a video
    - Read ‘Road.mp4’
    - Set left_roi and right_roi
        * LT and RB of left_roi : (200,400) , (600,600)
        * LT and RB of right_roi : (600,400), (1000,600)
- For each ROI, display canny edges
    - Use cvtColor to make a gray scale image
        * Use CV_BGR2GRAY
    - Use blur function with mask size as 5X5
    - For Thresholds for canny, use 10 and 60
- Your program should dsiplay two windwos
    - 'Left canny', 'Right canny'

# Result

![Result](/강의6/result.gif)