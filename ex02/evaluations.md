# Exercise 02: PmergeMe

As usual, there has to be enough tests to prove the program works as expected. If there isn't, do not grade this exercise. If any non-interface class is not in orthodox canonical class form, do not grade this exercise. 

### Code review

Check that a makefile is included with the usual compilation rules rules.

Check in the code that the program uses at least two containers.

If not, the evaluation stops here.

The person being evaluated must explain why they chose to use these containers
and not another?

Check in the code that the merge-insert sort algorithm is present and
is used for each container. The Ford-Johnson algorithm must be used.
A brief explanation is expected. In case of doubt, the evaluation stops here.

If one of the containers chosen here is included in one of the previous
exercises then the evaluation stops here.

### Main usage

You can now manually check that the program works correctly by using between
5 and 10 different positive integers of your choice as program arguments.

If this first test works and gives a sorted sequence of numbers you can continue.
If not, the evaluation stops now.

Now you have to check this operation by using the following command as an
argument to the program:

For linux:
`shuf -i 1-1000 -n 3000 | tr "\n" " " `

If the command works correctly, the person being evaluated should be able to
explain the difference in time used for each container selected.

If there are any problems during the execution and/or explanation then the
evaluation stops here.