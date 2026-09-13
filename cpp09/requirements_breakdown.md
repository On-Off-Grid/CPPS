This subject is a **progressive STL-and-algorithm module**: it starts with ordered lookup on historical data, moves to stack-based expression evaluation, and ends with a performance-oriented sorting exercise using two different containers and the Ford–Johnson merge-insert strategy. The real end goal is not just “make three programs pass,” but to make you think in terms of data structures, parsing, validation, algorithm choice, and container tradeoffs under C++98 constraints. 

## What the module wants

Module 09 requires you to use standard containers in each exercise, and once a container is used in one exercise, you cannot reuse it later in the module; exercise 02 must use at least two different containers. It also keeps the usual C++ module rules: compile with `c++ -Wall -Wextra -Werror`, stay compatible with C++98, avoid forbidden patterns like `using namespace`, and structure code cleanly with proper headers and Makefiles. 

The exercises are chained in difficulty very intentionally:
- ex00 teaches parsing + validation + ordered retrieval from a historical dataset.
- ex01 teaches expression evaluation with a LIFO model.
- ex02 teaches algorithmic design, recursion or staged decomposition, and performance comparison between containers. 

So the thought flow is: **read data correctly → choose the right container → model the operations naturally → handle bad input robustly → reason about complexity and implementation details**. That is exactly the kind of thinking you need later in exams and larger projects. 

## Exercise chain

| Exercise | Core task | Best mental model | Main thing you learn |
|---|---|---|---|
| ex00 BitcoinExchange | For each `date | value`, find the matching or closest earlier BTC rate and compute `value * rate`  
| ex01 RPN | Evaluate an inverted Polish expression using `+ - / *` and print the result  
| ex02 PmergeMe | Sort positive integers with merge-insert sort / Ford–Johnson using two containers and compare timing  

The exercises go from “use a container because it fits the data” to “use a container because it fits the execution model” to “use containers as part of algorithm analysis.” That progression is the hidden curriculum of the module. 

## ex00 BitcoinExchange

You must write a program named `btc` that takes one file argument, reads lines in the form `date | value`, validates them, and outputs the amount multiplied by the Bitcoin exchange rate from a CSV database; if the exact date does not exist, you must use the closest earlier date, not a later one. Valid dates use `YYYY-MM-DD`, and valid values are positive numbers between 0 and 1000, with appropriate error messages for invalid input.  

### What the exercise is really teaching

This is not just a file-reading exercise. It is teaching you that when data has a naturally sortable key like a date string in ISO format, you should exploit ordering rather than brute force scanning. Because `YYYY-MM-DD` compares lexicographically in calendar order, a sorted associative container becomes the natural fit. 

The conceptual problem is:

1. Load the reference database once.
2. For each query line:
   - parse it,
   - validate the date,
   - validate the numeric value,
   - find the exact or nearest lower date,
   - multiply and print. 

### Best container mindset

A strong fit here is an ordered associative container such as `std::map<std::string, float>` or `double`, because:
- the key is the date string,
- the DB stays sorted automatically,
- you can search for exact or nearest lower date efficiently using ordered lookup behavior.  

The key idea is “I need predecessor search,” not merely “I need to store pairs.”

### Why the date string works

The date format is fixed as `Year-Month-Day`. In this format:
- `"2011-01-03" < "2011-01-09"` is true lexicographically,
- `"2011-12-31" < "2012-01-01"` is also true lexicographically. 

That means you do **not** need a custom date object just to sort dates. You may still build a date validator, but ordering can remain string-based.

### Recommended architecture

Use a class like `BitcoinExchange` with responsibilities split cleanly:
- `loadDatabase(const std::string& filename)`
- `processInput(const std::string& filename)`
- `isValidDate(const std::string&)`
- `isValidValue(const std::string&, double&)`
- `getRateForDate(const std::string&)`
- small helpers for trimming and parsing

This split matters because peer evaluation often tests whether you understand the logic in pieces, not only whether your main works. 

### Parsing thought flow

For each input line:
1. Reject the header unless it is exactly the expected one.
2. Find the `|`.
3. Split left/right parts.
4. Trim spaces around both.
5. Validate date syntax.
6. Validate date semantics, such as real month/day ranges.
7. Parse the value.
8. Check non-negative and `<= 1000`.
9. Find exchange rate.
10. Print result. 

That order is important. You want to fail early and print the right error for the right reason.

### Date validation details

Be explicit here, because this is where many implementations get sloppy:
- Length must be 10.
- Characters at positions 4 and 7 must be `-`.
- Other characters must be digits.
- Year, month, day must be extracted as integers.
- Month must be 1 to 12.
- Day must be valid for that month.
- February must respect leap-year logic if you choose full correctness.

Even if the subject examples mostly show malformed obvious dates like `2001-42-42`, a peer can test subtler cases. 

A safe leap-year rule:
- leap if divisible by 4,
- except centuries unless divisible by 400.

### Value validation details

The subject says a valid value must be a float or positive integer between 0 and 1000, and example errors include negative numbers and very large numbers. That means your validation should distinguish:
- parse failure,
- negative value,
- too large value. 

A practical rule:
- parse using stream extraction,
- reject trailing garbage,
- then check:
  - `< 0` → `Error: not a positive number.`
  - `> 1000` → `Error: too large a number.`

### Closest earlier date lookup

This is the heart of ex00. The subject explicitly says if the input date is not in the DB, use the closest lower date, not the upper one. 

The mental model:
- If exact key exists, use it.
- Otherwise find the first element whose key is greater than the target.
- Then step one element back.
- If that greater element is already the beginning, there is no lower date available, so that is an error case you must define sensibly.

In map terms, this is predecessor lookup. That is why this exercise is really about ordered containers.

### Common mistakes

- Using a vector and linearly scanning the DB for every input line.
- Validating only the format, not real calendar values.
- Forgetting to trim spaces around `date | value`.
- Using the next greater date instead of the previous lower one.
- Storing rates as `int`.
- Printing generic `Error` instead of the expected meaningful message style shown in the subject. 

### What you should learn from ex00

You should come out of ex00 understanding: **when the query is “find exact or previous key,” an ordered map-like structure is the right tool**. You also learn that parsing and validation are part of the algorithm, not “annoying extras.” 

## ex01 RPN

You must write a program named `RPN` that takes an inverted Polish expression as one argument, supports the tokens `+ - / *`, prints the correct result to standard output, and prints an error to standard error if execution fails. The subject says the input numbers will always be less than 10, and you do not need to handle parentheses or decimal numbers. 

### What the exercise is really teaching

This exercise is about recognizing an execution model. Reverse Polish Notation is not best handled by precedence tables or recursive infix parsing. It is naturally evaluated by a **stack**:
- number → push
- operator → pop two operands, compute, push result
- end → one final value must remain 

So the hidden lesson is: **choose a container that mirrors the machine behavior**.

### Best container mindset

A stack-adapted structure is the natural fit. Whether you use `std::stack<int>` over an underlying container or implement LIFO behavior directly with another container, the core point is that only the top matters.

The evaluation rule is precise:
- read left to right,
- when token is a digit, push it,
- when token is an operator:
  - ensure at least two operands exist,
  - pop right operand,
  - pop left operand,
  - compute `left op right`,
  - push result. 

Order matters a lot for subtraction and division.

### Example reasoning

For `8 9 * 9 - 9 - 9 - 4 - 1 +`:
1. push 8
2. push 9
3. `*` → 72
4. push 9
5. `-` → 63
6. push 9
7. `-` → 54
8. push 9
9. `-` → 45
10. push 4
11. `-` → 41
12. push 1
13. `+` → 42 

That trace explains why a stack is not optional here; it is the algorithm.

### Parsing policy

The subject examples sometimes show expressions with no spaces in the rendered PDF text, but conceptually you should treat the argument as a token stream and design your parser carefully around the subject’s accepted use format. The safest student approach is usually to support space-separated tokens cleanly and reject malformed streams consistently, because the exercise is about correct processing, not heroic guessing. 

Your parser should reject:
- unknown characters,
- insufficient operands for an operator,
- extra operands left at the end,
- division by zero.

### Minimal algorithm

1. Iterate over tokens.
2. If token is a digit 0–9, push integer value.
3. If token is an operator:
   - ensure stack size at least 2,
   - pop `b`,
   - pop `a`,
   - push `a op b`.
4. At end, stack size must be exactly 1.


### Common mistakes

- Reversing operand order: doing `b - a` instead of `a - b`.
- Accepting multi-digit numbers when your parser logic was written only for chars.
- Printing errors to standard output instead of standard error.
- Forgetting to detect leftover values after finishing.
- Not checking division by zero. 

### What you should learn from ex01

This exercise should teach you to identify a computation pattern and map it directly to the right abstraction. ex00 was “ordered lookup”; ex01 is “LIFO evaluation.” The real skill is not memorizing containers, but seeing which one matches the behavior of the problem. 

## ex02 PmergeMe

You must write `PmergeMe`, accept a sequence of positive integers as arguments, sort them using merge-insert sort—explicitly the Ford–Johnson algorithm—handle errors on standard error, use at least two different containers, and process at least 3000 different integers. The output must show the sequence before sorting, after sorting, and the time taken for each of the two containers. 

### What the exercise is really teaching

This is the capstone. The subject is no longer only asking “can you use STL?” It is asking:
- can you implement a nontrivial algorithm,
- can you adapt it to more than one container,
- can you reason about performance,
- can you separate algorithmic logic from container-specific mechanics? 

The subject even strongly advises implementing the algorithm for each container rather than hiding everything behind one generic function. That means evaluators want to see that you understand the container behavior itself, not only template trickery. 

### The real end goal

The end goal of the whole module appears most clearly here: **you must stop thinking of containers as storage boxes and start thinking of them as algorithmic partners with specific costs and affordances**. ex02 forces you to compare two implementations on the same task and measure them. 

### Ford–Johnson in plain language

At a high level, merge-insert sort / Ford–Johnson works like this:

1. Group values into pairs.
2. In each pair, order the two values so the larger and smaller are known.
3. Build a “main chain” from one side of the pairs.
4. Recursively sort that main chain.
5. Insert the remaining elements into the right positions of that chain in a carefully chosen order.

The algorithm is famous because it minimizes comparisons better than simpler methods, which is why the subject explicitly names it instead of merely saying “sort somehow.” 

### A practical student mental model

Do not start from the formal paper definition. Start from this implementation model:

- Input: `a1 a2 a3 a4 ...`
- Build pairs: `(a1,a2) (a3,a4) ...`
- Inside each pair, sort locally so `small <= large`
- Collect all `large` values into a sequence `main`
- Recursively sort `main`
- Insert each `small` into the correct position relative to its paired `large`
- If there is an odd leftover element, insert it too

That gives you a concrete route to code.

### Why two containers matter

The subject requires at least two different containers and asks you to time both. That means the educational point is not only correctness, but also behavior:
- contiguous containers often have fast random access but expensive mid-insertions,
- linked containers have different traversal and insertion characteristics,
- recursion and binary-search-like insertion logic feel different depending on iterator category. 

So you should expect your implementation approach to differ slightly by container.

### Output requirements mindset

The subject asks for:
- first line: explicit text + unsorted sequence,
- second line: explicit text + sorted sequence,
- third line: explicit message with time for first container,
- last line: explicit message with time for second container. 

That means your program should:
- parse all input once,
- validate all numbers before timing,
- duplicate the data into both containers,
- run the algorithm separately,
- print both timings.

The timing should include the complete work, not only a tiny inner loop. The subject explicitly says the time must reflect all operations, including data management. 

### Error policy

You must reject invalid execution inputs, and duplicate handling is left to your discretion. That means duplicates are not mandated as an error by the subject, but your behavior must be intentional and explainable during evaluation. 

A strong policy is:
- reject non-positive values,
- reject non-numeric tokens,
- reject overflow beyond `int` if you store ints,
- either allow duplicates consistently or reject them consistently, but document your choice.

### Recommended implementation phases

#### Phase 1: input handling
- Parse argv from index 1 onward.
- Ensure each token is all digits or otherwise valid numeric text.
- Convert safely.
- Check `> 0`.
- Store into a base sequence. 

#### Phase 2: container duplication
- Copy the same numbers into container A and container B.
- This keeps benchmarking fair.

#### Phase 3: algorithm on one container
Implement and test on one container first:
- pair creation,
- local pair ordering,
- recursive main-chain sort,
- pending insertions,
- odd element handling.

#### Phase 4: port to second container
Re-implement the same algorithm with container-aware operations.

#### Phase 5: timing and verification
- Measure time before and after each sort.
- Verify output is sorted.
- Print as required. 

### Deep thought flow for the algorithm

Here is the most useful “how to think” version.

#### Step A: Pair the elements
From `[3, 5, 9, 7, 4]` build:
- `(3,5)`
- `(9,7)`
- leftover `4`

Then normalize pairs:
- `(3,5)`
- `(7,9)`
- leftover `4`

Now each pair gives:
- small sequence: `[3,7]`
- large sequence: `[5,9]`

#### Step B: Sort the large elements
Sort `[5,9]`. This becomes your main chain.

Why the large ones? Because each small one is guaranteed to be `<=` its partner large one, which gives you an insertion boundary intuition.

#### Step C: Insert the small elements
Insert `3` into `[5,9]` → `[3,5,9]`
Insert `7` into `[3,5,9]` before its correct place → `[3,5,7,9]`

#### Step D: Insert odd leftover
Insert `4` → `[3,4,5,7,9]`

That simplified example captures the structure before you refine the insertion ordering strategy.

### Where students usually struggle

- They implement “some merge sort” and call it Ford–Johnson.
- They do not clearly preserve pair relationships.
- They lose track of which small belongs to which large.
- They benchmark parsing plus one container but not the other fairly.
- They write one generic function without understanding iterator limitations.
- They get a correct sorted result but cannot explain why the algorithm is merge-insert. 

### Container choice strategy

Because you cannot reuse containers used earlier in the module, you must plan globally. The subject explicitly warns that containers used in earlier exercises become forbidden later. 

A sensible planning mindset is:
- ex00: use an ordered associative container for predecessor lookup.
- ex01: use a stack-oriented container.
- ex02: reserve two sequence containers appropriate for sorting experiments.

This is exactly why the subject says to read the entire module before starting. 

## What the full module is building in you

By the end of this subject, the model you are supposed to learn is:

1. **Understand the data shape.**
   - Historical date→value DB suggests ordered association.
   - RPN tokens suggest LIFO execution.
   - Sorting benchmark suggests sequence containers and container-aware algorithms. 

2. **Validate before computing.**
   - Bad input is part of the problem, not an edge afterthought. 

3. **Choose the container from the operations you need.**
   - predecessor search,
   - top push/pop,
   - pairing, traversal, insertion, timing. 

4. **Separate concerns.**
   - parsing,
   - data storage,
   - algorithm,
   - presentation/output,
   - error handling.

5. **Be able to explain the code aloud.**
   - The submission section warns that evaluators may request small modifications to verify your real understanding. 

That last point matters a lot: if you can only type the code but cannot explain why `lower_bound`-style logic solves ex00, why stack order matters in ex01, or how pair/main-chain/insertion works in ex02, you are not done learning the module. 

## How to code it yourself

A good self-driven approach is:

1. Build each exercise with a tiny manual test set first.
2. Write validation helpers before full logic.
3. Choose the container only after writing down the required operations.
4. For each class, write a short “responsibility note” in comments before coding.
5. After finishing, explain the algorithm to yourself without looking at the code.
6. Then make one small change, such as modifying an error case or output format, to prove you truly own the implementation. The subject explicitly says evaluation may include small modifications, so this is excellent practice. 

For ex00, start by making date parsing and predecessor lookup work on a tiny map. For ex01, test the stack logic on three or four small expressions before building the full parser. For ex02, prototype the pairing and insertion logic on paper with 5–7 numbers before writing any recursion. 

## A strong learning order

Use this order to maximize understanding:
- First, read and rewrite the requirements in your own words.
- Second, list the operations you need from the data structure.
- Third, choose the container.
- Fourth, implement validation.
- Fifth, implement the core algorithm.
- Sixth, test normal cases.
- Seventh, test malformed input aggressively.
- Eighth, rehearse your explanation aloud as if you were at peer evaluation. 

