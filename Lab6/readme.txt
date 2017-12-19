Sheldon Dong
004784870

readme.txt

For this assignment, most of my time was focused on the main.c file and the aspect of it
that required threading. THe original main.c file only allowed for one thread, and if any
other amount of threading was used, an error would be thrown saying that multithreading
is not allowed yet. The point of the HW was to fix this so multiple threads could be used 
in order to increase efficiency. To achieve this, I took out all the parts that was related
to printing out the pixel and put it into its own separate function in order to deal with
the syntax of the pthread_create function, which takes in only one function. I called this
function render. However, since this function relied on several variables that were local
to the main function, I had to make these variables global instead, such as scene and 
nthreads. The point of render was to have multiple threads each do its own part of rendering
the image. 

To achieve, this, I created a 3D array called scaled, which takes the width and height
dimensions, as well as the color of the pixel in the third dimension. Also, in order to 
multithread, I had to modify the other for loop and initialize px to the first thread_id, 
and increment by the number of threads we had. This breaks the image into sections that each
thread can handle by itself to render.

The reason for the 3D array of colors was to prevent race conditions. If one thread had
printed out of order of another thread, the image might not be in the correct order and
won't match the final image because of these race conditions. Storing each value in its
respective index and the printing out ensures that rendering is in the right order and the
same as if we were using one thread. 

I also had to use use pthread_join and join these threads that were created after threading
was completed, preventing any threads from being aborted too early.   
    
Performance wise, we can see that there are huge differences between the real times for each
test case and as we increase the number of threads.

time ./srt 1-test.ppm >1-test.ppm.tmp

real    0m43.627s
user    0m43.620s
sys     0m0.004s

time ./srt 2-test.ppm >2-test.ppm.tmp

real    0m22.371s
user    0m44.481s
sys     0m0.002s

time ./srt 4-test.ppm >4-test.ppm.tmp

real    0m11.247s
user    0m44.682s
sys     0m0.003s

time ./srt 8-test.ppm >8-test.ppm.tmp

real    0m5.769s
user    0m44.677s
sys     0m0.003s


As we can see, the unmodified srt with only 1 thread was the absolute slowest. We also see 
that all of the variance is in the real time. User and sys times are basically the same
throughout. The reason for the large discrepancies in real time is because since the
rendering is split up among the threads, as the number of threads increases, the more we 
parallelize the rendering, needing less time overall.     
