# Computer Vision with c++

## Video Segmentation and Counting the number of objects Drawing Bounding Rectangle

- Develop a program which performs pixel based background subtraction
    - Read ‘background.mp4’ as gray scale
    - Set background image as the average of the first 10 frames
    - Generate a binary image by using the following equation
        * 𝑅𝑒𝑠𝑢𝑙𝑡(𝑥,𝑦) =  
        { 255 |𝐶𝑢𝑟𝑟𝑒𝑛𝑡𝑓𝑟𝑎𝑚𝑒(𝑥,𝑦)−𝐵𝑎𝑐𝑘𝑔𝑟𝑜𝑢𝑛𝑑(𝑥,𝑦)|>20 }  
        { 0                 𝑂𝑡ℎ𝑒𝑟𝑤𝑖𝑠𝑒                 }  
- Draw bounding rectangle on the input video each moving object whose size is bigger than 400 pixels
- Print out the number of moving objects on the image whose size is bigger than 400 pixels

# Result

[Result](https://www.youtube.com/watch?v=8vijUOJ0-68)