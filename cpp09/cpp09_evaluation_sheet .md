# C++ Module 09 — Evaluation Sheet

---

## Exercise 00: Bitcoin Exchange

For this first exercise, you have to find a makefile with the usual compilation rules and the files requested in the subject.

### Code review

- Check that a makefile is present with the usual compilation rules.
- Check in the code that the program uses at least one container.
- The person being evaluated must explain why they chose to use this container and not another.
- If not, the evaluation stops here.

**Result:** &nbsp;&nbsp; **Yes** [ &nbsp;&nbsp; ] &nbsp;&nbsp;&nbsp;&nbsp; **No** [ &nbsp;&nbsp; ]

---

### Error handling

You must be able to use an empty file or a file with errors (a basic example exists in the subject). The program must not stop its execution before having performed the operations on the whole file passed as argument.

You can:

- use a wrong date;
- enter a value greater than 1000 or less than 0.

If there is any problem during the execution then the evaluation stops here.

**Result:** &nbsp;&nbsp; **Yes** [ &nbsp;&nbsp; ] &nbsp;&nbsp;&nbsp;&nbsp; **No** [ &nbsp;&nbsp; ]

---

### Main usage

You must now use the `input.csv` file located at the top of this page. You can modify this file with the values you want.

You have to run the program with the `input.csv` file as parameter. Please compare some dates manually with the specified value.

If the date does not exist in the database, the program will have to use the nearest lower date.

**Result:** &nbsp;&nbsp; **Yes** [ &nbsp;&nbsp; ] &nbsp;&nbsp;&nbsp;&nbsp; **No** [ &nbsp;&nbsp; ]

---

## Exercise 01: Reverse Polish Notation

For this second exercise, you have to find a makefile with the usual compilation rules and the files requested in the subject.

### Code review

- Check that a makefile is present with the usual compilation rules.
- Check in the code that the program uses at least one container.
- The person being evaluated must explain why they chose to use this container and not another.
- If not, the evaluation stops here.
- If the container chosen here is present in the first exercise then the evaluation stops here.

**Result:** &nbsp;&nbsp; **Yes** [ &nbsp;&nbsp; ] &nbsp;&nbsp;&nbsp;&nbsp; **No** [ &nbsp;&nbsp; ]

---

### Main usage

Check that the program runs correctly using different formulas of your choice. The program is not required to handle expressions with parenthesis or decimal numbers.

If there is any problem during the execution then the evaluation stops here.

**Result:** &nbsp;&nbsp; **Yes** [ &nbsp;&nbsp; ] &nbsp;&nbsp;&nbsp;&nbsp; **No** [ &nbsp;&nbsp; ]

---

### Usage advanced

Check that the program runs correctly using different formulas of your choice.

Here are some tests:

```text
8 9 * 9 - 9 - 9 - 4 - 1 +
  > Result: 42

9 8 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -
  > Result: 42

1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 / 
  > Result: 15
```

You can use the examples in the topic if you don't know which formula to use.

If there is any problem during the execution then the evaluation stops here.

**Result:** &nbsp;&nbsp; **Yes** [ &nbsp;&nbsp; ] &nbsp;&nbsp;&nbsp;&nbsp; **No** [ &nbsp;&nbsp; ]

---

## Exercise 02: PmergeMe

As usual, there has to be enough tests to prove the program works as expected. If there isn't, do not grade this exercise. If any non-interface class is not in orthodox canonical class form, do not grade this exercise.

### Code review

- Check that a makefile is included with the usual compilation rules.
- Check in the code that the program uses at least two containers.
  - If not, the evaluation stops here.
- The person being evaluated must explain why they chose to use these containers and not another.
- Check in the code that the merge-insert sort algorithm is present and is used for each container. The Ford–Johnson algorithm must be used.
  - A brief explanation is expected. In case of doubt, the evaluation stops here.
- If one of the containers chosen here is included in one of the previous exercises then the evaluation stops here.

**Result:** &nbsp;&nbsp; **Yes** [ &nbsp;&nbsp; ] &nbsp;&nbsp;&nbsp;&nbsp; **No** [ &nbsp;&nbsp; ]

---

### Main usage

You can now manually check that the program works correctly by using between 5 and 10 different positive integers of your choice as program arguments.

If this first test works and gives a sorted sequence of numbers you can continue. If not, the evaluation stops now.

Now you have to check this operation by using the following command as an argument to the program:

**For Linux:**

```bash
shuf -i 1-1000 -n 3000 | tr "\n" " "
```

**For macOS:**

```bash
jot -r 3000 1 1000 | tr '\n' ' '
```

If the command works correctly, the person being evaluated should be able to explain the difference in time used for each container selected.

If there are any problems during the execution and/or explanation then the evaluation stops here.

**Result:** &nbsp;&nbsp; **Yes** [ &nbsp;&nbsp; ] &nbsp;&nbsp;&nbsp;&nbsp; **No** [ &nbsp;&nbsp; ]

---

## Notes

Use this section to record:

- container choices for each exercise;
- key points from the student's explanations;
- any edge cases you tested;
- any issues encountered during evaluation.

---