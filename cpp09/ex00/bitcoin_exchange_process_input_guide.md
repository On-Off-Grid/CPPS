# Understanding `processInput(...)` in BitcoinExchange

This guide explains the logic and syntax behind the `processInput(...)` function, with special focus on:

- why `std::map` is a good container for the Bitcoin database;
- what a `std::map` iterator represents;
- how `upper_bound()` locates the first date after the requested date;
- why decrementing the iterator gives the nearest lower date.

The relevant implementation follows this structure:

```cpp
void BitcoinExchange::processInput(const std::string& inputPath) const
{
    std::ifstream inputFile(inputPath.c_str());

    if (!inputFile.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;

    while (std::getline(inputFile, line)) {
        std::string trimmedLine = trim(line);

        if (trimmedLine.empty())
            continue;

        if (trimmedLine == "date | value")
            continue;

        std::size_t pipePos = line.find('|');

        if (pipePos == std::string::npos) {
            std::cout << "Error: bad input => "
                      << trimmedLine << std::endl;
            continue;
        }

        std::string dateStr = trim(line.substr(0, pipePos));
        std::string valStr = trim(line.substr(pipePos + 1));

        if (!isValidDate(dateStr)) {
            std::cout << "Error: bad input => "
                      << dateStr << std::endl;
            continue;
        }

        double value;
        std::string errorMsg;

        if (!parseValue(valStr, value, errorMsg)) {
            std::cout << "Error: " << errorMsg << std::endl;
            continue;
        }

        std::map<std::string, double>::const_iterator it;
        it = database.upper_bound(dateStr);

        if (it == database.begin()) {
            std::cout << "Error: bad input => "
                      << dateStr << std::endl;
        }
        else {
            --it;
            std::cout << dateStr << " => " << valStr
                      << " = " << value * it->second << std::endl;
        }
    }

    inputFile.close();
}
```

The names and exact error messages may differ in a student implementation, but the algorithmic structure is the important part.

---

## 1. What problem does `processInput` solve?

The database contains historical exchange rates. Conceptually, it looks like this:

```text
2011-01-03 -> 0.30
2011-01-04 -> 0.32
2011-01-09 -> 0.32
2012-01-11 -> 7.10
```

The input file contains queries:

```text
2011-01-03 | 3
2011-01-09 | 1
2011-01-10 | 2
```

For each query, the program must:

1. read the date;
2. read the Bitcoin amount;
3. validate both values;
4. find the exchange rate for that date;
5. if the exact date is missing, use the closest earlier date;
6. multiply the amount by the selected rate;
7. print the result;
8. continue processing the rest of the file even if this line is invalid.

The key operation is step 5: **find the greatest database date that is less than or equal to the requested date**.

Mathematically, if the requested date is `D`, the desired database key is:

\[
\max\{K \mid K \leq D\}
\]

This is a predecessor lookup in an ordered set of keys.

---

## 2. Why use `std::map`?

A `std::map` stores key-value pairs in sorted key order.

For this project, the natural type is:

```cpp
std::map<std::string, double> database;
```

Each element contains:

```cpp
key   = date
value = exchange rate
```

For example:

```cpp
database["2011-01-03"] = 0.30;
database["2011-01-04"] = 0.32;
database["2011-01-09"] = 0.32;
```

The map keeps the keys ordered automatically:

```text
2011-01-03
2011-01-04
2011-01-09
```

That ordering is exactly what is needed for nearest-date lookup.

### Why date strings sort correctly

The subject uses the format:

```text
YYYY-MM-DD
```

Because the most significant part comes first, ordinary string comparison gives chronological ordering:

```text
"2011-01-03" < "2011-01-09"
"2011-12-31" < "2012-01-01"
```

This would not be true for a format such as:

```text
DD-MM-YYYY
```

For example, comparing `31-12-2011` and `01-01-2012` as strings would not represent chronological order. The ISO-like format used by the subject avoids that problem.

### Why not a vector?

A vector could store the records, but lookup would usually require:

```text
scan every date until the correct position is found
```

That is a linear search, with complexity approximately \(O(n)\) per query unless you write and maintain your own sorted-search logic.

A map is designed for ordered key lookup. Its search operations are typically logarithmic:

\[
O(\log n)
\]

where `n` is the number of database entries.

### Why not an unordered map?

An `unordered_map` is optimized for exact key lookup, but it does not keep dates in sorted order. It can answer:

```text
Does this exact date exist?
```

but it cannot naturally answer:

```text
What is the closest earlier date?
```

You would need additional logic, such as scanning all keys or maintaining a separate sorted structure. A regular ordered `std::map` directly represents the problem.

### Why not a list?

A list preserves sequence order but does not provide efficient ordered lookup. You would need to traverse it manually, and it does not provide the same ordered-search operations as `std::map`.

### The important design principle

The correct explanation is not merely:

> I used a map because the subject requires a container.

A better explanation is:

> The database is organized by dates, and each query requires exact lookup or predecessor lookup. `std::map` keeps the dates sorted and provides ordered-search operations, so it naturally supports finding the closest date that is not greater than the query date.

---

## 3. Understanding the map element type

For:

```cpp
std::map<std::string, double> database;
```

each element is a `std::pair`:

```cpp
std::pair<const std::string, double>
```

The map element contains:

```cpp
first  -> key   -> date
second -> value -> exchange rate
```

Therefore, if `it` is an iterator pointing to one database entry:

```cpp
it->first
```

means:

```text
the date stored at this position
```

and:

```cpp
it->second
```

means:

```text
the exchange rate stored at this position
```

For example:

```cpp
std::cout << it->first;   // date
std::cout << it->second;  // rate
```

This is equivalent to writing:

```cpp
std::cout << (*it).first;
std::cout << (*it).second;
```

The arrow syntax is simply more convenient.

---

## 4. What is an iterator?

An iterator behaves similarly to a pointer into a container.

For example:

```cpp
std::map<std::string, double>::const_iterator it;
it = database.begin();
```

`it` does not contain the date itself. It represents a position inside the map.

If the map is:

```text
2011-01-03 -> 0.30
2011-01-04 -> 0.32
2011-01-09 -> 0.32
```

then the iterator may point to one of those entries:

```text
it
|
v
2011-01-04 -> 0.32
```

You access the entry through:

```cpp
it->first
it->second
```

Moving the iterator changes which map element it refers to:

```cpp
++it;  // move forward to the next key
--it;  // move backward to the previous key
```

For a `std::map`, iterators move through keys in sorted order.

---

## 5. `begin()` and `end()`

### `begin()`

```cpp
database.begin()
```

returns an iterator pointing to the first, smallest key.

Example:

```text
begin()
   |
   v
2011-01-03 -> 0.30
2011-01-04 -> 0.32
2011-01-09 -> 0.32
```

### `end()`

```cpp
database.end()
```

returns an iterator representing the position just after the final element.

It does not point to a valid map entry.

Example:

```text
2011-01-03 -> 0.30
2011-01-04 -> 0.32
2011-01-09 -> 0.32
                              ^
                              end()
```

You must never dereference `end()`:

```cpp
// Wrong: there is no element at end()
std::cout << database.end()->second;
```

`end()` is useful as a boundary or as the result of some lookup operations, but it is not an actual element.

---

## 6. Understanding `upper_bound(dateStr)`

The key line is:

```cpp
std::map<std::string, double>::const_iterator it;
it = database.upper_bound(dateStr);
```

`upper_bound(key)` returns an iterator to the **first element whose key is strictly greater than `key`**.

The word “strictly” is important:

```text
returned key > requested key
```

It does not return a key equal to the requested key.

Suppose the database is:

```text
2011-01-03 -> 0.30
2011-01-04 -> 0.32
2011-01-09 -> 0.32
2012-01-11 -> 7.10
```

### Query 1: exact date exists

```text
requested date = 2011-01-04
```

`upper_bound("2011-01-04")` returns the first date greater than it:

```text
2011-01-09 -> 0.32
^
it points here
```

The exact date is immediately before that iterator. Decrementing gives:

```text
2011-01-04 -> 0.32
```

### Query 2: date lies between two database dates

```text
requested date = 2011-01-06
```

The first database date greater than it is:

```text
2011-01-09 -> 0.32
^
it points here
```

The nearest lower date is immediately before it:

```text
2011-01-04 -> 0.32
^
--it points here
```

That is exactly the date the subject requires.

### Query 3: query is after the final database date

```text
requested date = 2013-01-01
```

There is no database date greater than it, so:

```cpp
it == database.end()
```

The element immediately before `end()` is the last database record. Decrementing is valid as long as the map is not empty:

```cpp
--it;
```

### Query 4: query is before the first database date

```text
requested date = 2009-01-01
```

The first database date is already greater than the query, so:

```cpp
it == database.begin()
```

There is no lower date to use. Decrementing would be invalid.

That is why the code checks this before decrementing:

```cpp
if (it == database.begin()) {
    // no valid earlier date exists
}
else {
    --it;
    // safe to use it
}
```

---

## 7. Why decrementing gives the nearest lower value

This works because the map is sorted.

Imagine the ordered keys as positions on a number line:

```text
2011-01-03    2011-01-04    2011-01-09    2012-01-11
       |             |             |              |
```

For a query date `2011-01-06`, `upper_bound()` finds the first key strictly after it:

```text
2011-01-03    2011-01-04    2011-01-06    2011-01-09
       |             |             |              |
                                  query          upper_bound
```

The previous map element must be the largest key smaller than the query:

```text
2011-01-04
```

In iterator terms:

```cpp
it = database.upper_bound("2011-01-06");
--it;
```

After these two operations, `it` points to the nearest lower date.

The logic can be stated formally:

```text
upper_bound(D) = first key K such that K > D
previous element = greatest key K such that K <= D
```

That previous element is the required exchange-rate record.

---

## 8. Why `upper_bound` is better than `lower_bound` here

`lower_bound(key)` returns the first key that is greater than or equal to `key`.

`upper_bound(key)` returns the first key that is strictly greater than `key`.

For this problem, `upper_bound` makes the predecessor operation uniform:

```cpp
it = database.upper_bound(dateStr);

if (it == database.begin()) {
    // no lower or equal date exists
}
else {
    --it;
    // exact date or nearest lower date
}
```

With `lower_bound`, you would need an additional case:

```cpp
it = database.lower_bound(dateStr);

if (it != database.end() && it->first == dateStr) {
    // exact match: use it
}
else if (it == database.begin()) {
    // no lower date exists
}
else {
    --it;
    // nearest lower date
}
```

Both approaches can work, but `upper_bound` is elegant here because the element immediately before the result is always the desired exact-or-lower entry.

---

## 9. The important boundary case

Never write this without checking the boundary:

```cpp
it = database.upper_bound(dateStr);
--it;
```

If the query is before the first database date, `upper_bound()` returns `database.begin()`. Decrementing `begin()` is invalid and causes undefined behavior.

Correct structure:

```cpp
it = database.upper_bound(dateStr);

if (it == database.begin()) {
    std::cout << "Error: no earlier date exists" << std::endl;
}
else {
    --it;
    double rate = it->second;
    // use rate
}
```

Also consider the empty-map case. If the database failed to load or contains no valid records, then no lookup should be attempted.

A robust implementation ensures the database is non-empty before calling `processInput()`.

---

## 10. Explaining the complete function flow

The function processes the input file line by line.

### Open the file

```cpp
std::ifstream inputFile(inputPath.c_str());
```

In C++98, `std::ifstream` commonly receives a C-style string, so `.c_str()` converts the `std::string` path into the expected form.

### Stop if opening failed

```cpp
if (!inputFile.is_open()) {
    std::cerr << "Error: could not open file." << std::endl;
    return;
}
```

There is no point trying to read from a file that was not opened.

### Read one line at a time

```cpp
while (std::getline(inputFile, line)) {
```

`std::getline` reads one complete line, excluding the newline character.

The loop continues until the end of the file or a read failure.

### Ignore harmless lines

```cpp
std::string trimmedLine = trim(line);

if (trimmedLine.empty())
    continue;

if (trimmedLine == "date | value")
    continue;
```

The `continue` statement skips the rest of the current iteration and moves to the next line. This lets the program ignore blank lines and the header without terminating the entire process.

### Find the separator

```cpp
std::size_t pipePos = line.find('|');
```

The input format is:

```text
date | value
```

The pipe character divides the date from the requested Bitcoin amount.

If the pipe is not found:

```cpp
if (pipePos == std::string::npos) {
    // malformed line
    continue;
}
```

`std::string::npos` means “not found.”

### Split the line

```cpp
std::string dateStr = trim(line.substr(0, pipePos));
std::string valStr = trim(line.substr(pipePos + 1));
```

If the line is:

```text
2011-01-03 | 3
```

and the pipe is at position 11:

```cpp
line.substr(0, pipePos)
```

extracts the part before the pipe:

```text
2011-01-03
```

Then:

```cpp
line.substr(pipePos + 1)
```

extracts everything after the pipe:

```text
 3
```

`trim()` removes surrounding spaces.

### Validate the date

```cpp
if (!isValidDate(dateStr)) {
    // print error
    continue;
}
```

The function should reject incorrect syntax and impossible dates, such as:

```text
2012-42-42
2012-02-31
```

### Parse and validate the value

```cpp
double value;
std::string errorMsg;

if (!parseValue(valStr, value, errorMsg)) {
    // print the appropriate error
    continue;
}
```

The value must be numeric and within the allowed range:

```text
0 <= value <= 1000
```

The key point is that invalid lines are skipped with `continue`; they do not stop processing of later valid lines.

### Locate the exchange rate

```cpp
std::map<std::string, double>::const_iterator it;
it = database.upper_bound(dateStr);
```

This is the central map operation.

### Handle the lower-boundary case

```cpp
if (it == database.begin()) {
    // The query is earlier than the first database date.
}
```

No valid earlier or equal date exists.

### Decrement to the predecessor

```cpp
else {
    --it;
```

Now `it` points to the greatest database date that is less than or equal to `dateStr`.

### Access the exchange rate

```cpp
double rate = it->second;
```

Because every map element is a key-value pair:

```cpp
it->first  // selected date
it->second // selected exchange rate
```

### Calculate and print

```cpp
std::cout << dateStr << " => " << valStr
          << " = " << value * rate << std::endl;
```

The requested Bitcoin amount is multiplied by the selected exchange rate.

---

## 11. Worked example

Suppose the database contains:

```text
2011-01-03 -> 0.30
2011-01-04 -> 0.32
2011-01-09 -> 0.32
2012-01-11 -> 7.10
```

Input:

```text
2011-01-06 | 2
```

### Step 1: lookup

```cpp
it = database.upper_bound("2011-01-06");
```

The first key greater than `2011-01-06` is:

```text
2011-01-09
```

So `it` points to:

```text
2011-01-09 -> 0.32
```

### Step 2: decrement

```cpp
--it;
```

Now `it` points to:

```text
2011-01-04 -> 0.32
```

This is the nearest lower date.

### Step 3: calculate

```text
2 * 0.32 = 0.64
```

Expected style of output:

```text
2011-01-06 => 2 = 0.64
```

---

## 12. Why the selected date is not always exact

A common misunderstanding is to assume that the program must find an exact date. The subject explicitly allows missing dates and requires the nearest earlier database date.

For example:

```text
Database dates:  2011-01-03, 2011-01-09
Query date:      2011-01-06
```

The program must choose:

```text
2011-01-03
```

It must not choose:

```text
2011-01-09
```

because that date is later than the query.

The map iterator operation expresses this rule directly:

```cpp
upper_bound(queryDate)  // first date too late
--it                    // previous date is the answer
```

---

## 13. What should be explained during evaluation?

A concise but strong explanation is:

> I use `std::map<std::string, double>` because the database is keyed by dates and the dates are stored in sortable `YYYY-MM-DD` format. The map maintains the dates in ascending order and provides `upper_bound`, which returns the first date strictly greater than the query. The required date is the exact date or the closest earlier date, so I decrement the iterator returned by `upper_bound`. If the iterator equals `begin()`, the query is earlier than every database entry and decrementing would be invalid, so I handle that case first. The selected rate is accessed with `it->second`.

That explanation demonstrates understanding of:

- the data model;
- the reason for choosing `map`;
- ordered lookup;
- iterator semantics;
- the purpose of decrementing;
- the boundary condition.

---

## 14. Common mistakes

### Decrementing without checking `begin()`

```cpp
it = database.upper_bound(dateStr);
--it; // unsafe if it == database.begin()
```

### Dereferencing `end()`

```cpp
it = database.upper_bound(dateStr);
std::cout << it->second; // unsafe if it == database.end()
```

In this particular algorithm, `end()` can be safely decremented if the map is non-empty, but it must not be dereferenced before decrementing.

### Using the upper date

For a query between two records, using the result of `upper_bound()` directly would select a date that is too late.

### Using `lower_bound()` incorrectly

`lower_bound()` returns the first date greater than or equal to the query. If the date is missing, that is the upper date, not the lower date. You need either:

```cpp
upper_bound() followed by --it
```

or additional exact-match logic with `lower_bound()`.

### Searching an unordered container

An unordered map cannot naturally find the nearest lower date because it does not maintain chronological order.

### Stopping after the first invalid line

The function should report the error and continue processing the remaining lines. That is why validation failures use `continue` rather than `return`.

---

## Summary of the iterator algorithm

The complete nearest-date logic is:

```cpp
std::map<std::string, double>::const_iterator it;
it = database.upper_bound(queryDate);

if (it == database.begin()) {
    // No database date is <= queryDate.
    // Handle the error.
}
else {
    --it;
    // it now points to the exact date or nearest lower date.
    double exchangeRate = it->second;
}
```

In one sentence:

> `upper_bound()` finds the first date that is too late; decrementing its iterator moves to the last date that is not too late.
