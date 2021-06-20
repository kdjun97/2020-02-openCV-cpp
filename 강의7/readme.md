# Computer Vision with c++

## Project2

- Develop a program which draws lines corresponding to lanes of a video
    - Read ‘Road-1.mp4’ (강의6)
    - Use left_roi and right_roi defined in assignment 6
    - Perform line filtering and line merging
        - Line filtering: Use angle information of lines
            * Left_roi : Use lines whose angle is between 30 and 60 degress
            * Right_roi : Use lines whose angle is between 120 and 150 degress
        - Line merging
            * Take average of rho and theta of filtered lines
            * After line merging there should be one left line and one right line
    - The color of lines should be red
    - Each line should be reached to the boundary of a frame

# Result

![Result](/강의7/result.gif)