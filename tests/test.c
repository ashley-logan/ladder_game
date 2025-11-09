/*
Define a makefile rule that copies the 
functions that you want to test from main.c
into a test case file inside a subdirectory 
where all testing is compiled and run; that is,
the main.c functions are copied into tests/test.c, 
which also have a test case function (e.g. 
test_countWordsOfLength()) for each function (e.g. 
countWordsOfLength()) copied in, and also has 
its own main() that calls all of the test_*() 
functions. This subdirectory (tests/) also may 
have its own makefile and is where the test.c is 
compiled and the testing suite is executed. This 
option is nice since it keeps the primary main.c 
application clean, putting all testing in a separate 
location. The major downside to this option is having 
multiple copies of the same code, which needs to be 
managed carefully.
*/

