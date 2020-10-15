Language Choice

1. Low level because the algorithm needs do a lot of computations quickly and hence would benefit from a lot of low-level tweaking.  Performance enhancement is generally easier in low-level languages.

2. Because the time-scale is so large a high level language would be suitable.  

3. Experimenting favors the use of high-level languages because it would result in shorter code, and more easily accomplishes the purpose of seeing whether an algorithm works or not.

4. It doesn't seem like the software needs to get very performant, like it would if the number of views were a lot larger.  Hence a high level language would be suitable here.

5. Low level because reliability is very key here, and would have the advantage of being quicker to deploy the airbag and save someone from harm more reliably.

Data Types

1. Integer
2. Floating point
3. Integer, from a user standpoint--A driver generally only needs a rounded integer number to gauge his/her speed
4. Integer
5. Floating point

Loops

1. For 
2. while
3. for--the act of querying doesn't seem to require a loop.  However for something like our argument parsing from the command line, since we know how many files are required we can use a for loop for any related operations.
4. For--can test primatlity of each number from 1 to 1000
5. For 

Sequence types

1. Tuple--won't change
2. Dictionary--keys are cities, rainfall as values
3. Tuple--not likely to change
4. List--likely to change.  Since I'm only storing one coordinate pair a list is suitable.
5. Dictionary--keys are usernames, data are values. 

Conditionals

The issue might be with the seed value of random.random().  Given the first if statement doesn't trigger, there is ~23% chance of histogram[2] += 1.

Mutable or immutable

Because we are able to reassign elements of a and b the weevil container is mutable.


