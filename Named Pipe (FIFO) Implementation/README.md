<h1>Named Pipe (FIFO) Implementation</h1>

<h2>Named Pipe</h2>
<p>It is an extension of the traditional Pipe concept in unix programming system. It is used for communication in two diffrent process that is not created by him or not an child or grand child process. It is also called FIFO.</p>

<p>It has the one interesting thing that it cannot be use on one end only if any process wnat to use it than it has to be open from both side liek Read or Write than it Read from one end and write from another end</p>

<p>If FIFO file will be opened as read or write both at together than it will worked as LIFO concept</p>
<br>


<p>Syntax to Open create FIFO file -  <i> int mkfifo(const char *pathname, mode_t mode); </i></p1>

<h2>Command for run the programm</h2>

<li>g++ FIFO_W.cpp</li>
<li>g++ FIFO_R.cpp</li>

<br>
<h3>Programm Description</h3>
<p>This Programm is example to show the communication in two process. One process(programm) take input string from file and write the output on FIFO file and another process(programm) Rede the content and invert them all and write them on file named as output.txt</p>

<br>

<h2>IO Files</h2>
<li>input.txt</li>
<li>output.txt</li>

<h3>Author : Aditya Kumawat</h3>