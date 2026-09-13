# C++ Module 09 — Algorithms, Containers, and Logic

> A deep-dive reference for the three exercises: **BitcoinExchange**, **RPN**, and **PmergeMe**.
> Use this to understand *why* each decision was made — the kind of explanation an evaluator expects.

---

## Container Allocation Map

No container may be reused across exercises. The four containers chosen are:

| Exercise | Container | Why it fits |
|---|---|---|
| ex00 — `btc` | `std::map<std::string, double>` | Ordered by key; supports efficient predecessor search |
| ex01 — `RPN` | `std::list<double>` (used as LIFO stack) | Cheap push/pop at tail; LIFO model for stack evaluation |
| ex02 — `PmergeMe` | `std::vector<Element>` | Contiguous memory, fast random access and `lower_bound` |
| ex02 — `PmergeMe` | `std::deque<Element>` | Segmented memory, O(1) front/back ops, different cache profile |

---

## Exercise 00 — BitcoinExchange

### The problem in one sentence
> For each `date | value` line in an input file, find the **closest earlier or equal** Bitcoin exchange rate in a CSV database and print `value × rate`.

### Why `std::map`?

A `std::map<std::string, double>` stores date strings as keys in **sorted (lexicographic) order**.
Because the date format is fixed as `YYYY-MM-DD`, lexicographic order *is* calendar order:

```
"2011-01-03" < "2011-01-09" < "2011-12-31" < "2012-01-01"  ✓
```

This means you never need a custom date comparator for ordering — the default `std::less<std::string>` works perfectly.

### Predecessor lookup — the heart of ex00

```
goal: given a query date Q, find the greatest key K in the map such that K ≤ Q
```

The algorithm is two steps using `std::map::upper_bound`:

```
┌────────────────────────────────────────────────────────────────────────────┐
│  it = map.upper_bound(Q)                                                   │
│     → points to the first key STRICTLY GREATER than Q                     │
│                                                                            │
│  if (it == map.begin())  → no key ≤ Q exists → error                      │
│  else { --it; use it->second as the rate; }                               │
└────────────────────────────────────────────────────────────────────────────┘
```

Visually, for DB = `[2009-01-02, 2011-01-03, 2011-01-09, 2012-01-11]`:

```
Query: 2011-01-05
                                  ↓ upper_bound points here (2011-01-09)
  [2009-01-02] [2011-01-03] [2011-01-09] [2012-01-11]
                      ↑ --it steps back here → rate used
```

This is O(log N) per query. A linear scan over a vector would be O(N) per query — wrong tool for this problem.

### Parsing pipeline (order matters)

```
Input line: " 2011-01-03 | 1.5 "

 1. Find '|'           → split into date-side and value-side
 2. Trim both sides    → "2011-01-03"  and  "1.5"
 3. Validate date      → length=10, dashes at [4],[7], all-digits elsewhere,
                         month in [1..12], day in [1..daysInMonth(month)]
 4. Validate value     → leading '-' → "not a positive number."
                         parse double, reject trailing garbage ("1.2.3", "1 2")
                         > 1000 → "too large a number."
 5. Lookup predecessor → upper_bound - 1
 6. Print result
```

Failing at step 3 → `Error: bad input => <date>`  
Failing at step 4 → specific message per cause  
Failing at step 5 → `Error: bad input => <date>` (date precedes all DB entries)

### Date validation — leap year rule

```cpp
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
```

| Year | Rule applied | Leap? |
|---|---|---|
| 2000 | divisible by 400 | ✓ yes |
| 1900 | divisible by 100, not by 400 | ✗ no |
| 2024 | divisible by 4, not by 100 | ✓ yes |
| 2100 | divisible by 100, not by 400 | ✗ no |

Feb 29 of a non-leap year → rejected as "bad input".

### Validated edge cases

| Input | Expected | Result |
|---|---|---|
| `2011-02-29` | bad date (2011 not leap) | `Error: bad input => 2011-02-29` ✓ |
| `2012-02-29` | valid (2012 is leap) | computed ✓ |
| `2000-02-29` | valid date, but no DB entry before 2000 | `Error: bad input => 2000-02-29` ✓ |
| `1900-02-29` | bad date (1900 not leap) | `Error: bad input => 1900-02-29` ✓ |
| `| -0` | negative number | `Error: not a positive number.` ✓ |
| `| +5` | positive, accepted | computed ✓ |
| `| 1.2.3` | trailing garbage | `Error: bad input => 1.2.3` ✓ |
| `| 1000` | boundary, accepted | computed ✓ |
| `| 1000.1` | too large | `Error: too large a number.` ✓ |
| no `|` in line | malformed | `Error: bad input => ...` ✓ |
| Date before earliest DB entry | no predecessor | `Error: bad input => <date>` ✓ |

> [!NOTE]
> `-0` is caught by checking if the **raw string** starts with `'-'` before parsing into a `double`, because IEEE 754 `-0.0 == 0.0` and would otherwise pass the `val < 0` check.

---

## Exercise 01 — Reverse Polish Notation (RPN)

### The problem in one sentence
> Evaluate a space-separated RPN expression left-to-right using a LIFO stack; print the result or `Error` to stderr.

### Why `std::list` as the stack?

A **stack** (Last-In-First-Out) is the direct computational model for RPN:
- a number → push onto stack
- an operator → pop two operands, compute, push result

`std::stack<T>` is excluded (already-used containers would reduce choice). `std::list<double>` exposes the same LIFO interface through `push_back`, `back`, and `pop_back`, operating only on the tail — no random access ever needed.

**Contrast with a vector**: a vector also works but has amortized O(1) push. A list always O(1) and illustrates the structural choice: when you only care about the tail, a linked list makes the intent explicit.

### Stack trace walkthrough

```
Expression: "8 9 * 9 - 9 - 9 - 4 - 1 +"

Token  Action          Stack (left=bottom, right=top)
─────  ──────────────  ──────────────────────────────
8      push 8          [8]
9      push 9          [8, 9]
*      pop 9, pop 8    [72]      (8 * 9 = 72)
9      push 9          [72, 9]
-      pop 9, pop 72   [63]      (72 - 9 = 63)
9      push 9          [63, 9]
-      pop 9, pop 63   [54]      (63 - 9 = 54)
9      push 9          [54, 9]
-      pop 9, pop 54   [45]      (54 - 9 = 45)
4      push 4          [45, 4]
-      pop 4, pop 45   [41]      (45 - 4 = 41)
1      push 1          [41, 1]
+      pop 1, pop 41   [42]      (41 + 1 = 42)

Stack has exactly 1 element → result = 42 ✓
```

### Operand order — critical for `-` and `/`

```
For operator OP:
  b = stack.pop_back()   ← right operand (was pushed LAST)
  a = stack.pop_back()   ← left operand  (was pushed FIRST)
  push(a OP b)

"5 3 -" means 5 - 3 = 2  (NOT 3 - 5 = -2)
"9 3 /" means 9 / 3 = 3  (NOT 3 / 9 = 0.33)
```

The first element popped is the **right-hand side** because it was pushed more recently.

### Parsing rules

| Token | Valid? | Action |
|---|---|---|
| Single digit `0`–`9` | ✓ | Push as integer |
| `+`, `-`, `*`, `/` | ✓ | Pop 2, compute, push |
| Multi-char (e.g. `12`) | ✗ | `Error` |
| Unknown char (e.g. `%`) | ✗ | `Error` |
| Operator with < 2 on stack | ✗ | `Error` |
| Division by zero | ✗ | `Error` |
| Leftovers at end (≠ 1 item) | ✗ | `Error` |

> [!IMPORTANT]
> **`Error` must go to `stderr`**, not `stdout`. The evaluation sheet tests this implicitly — if your program pipes `Error` through, a tester can detect stdout is clean.

### Validated edge cases

| Expression | Expected | Result |
|---|---|---|
| `"8 9 * 9 - 9 - 9 - 4 - 1 +"` | 42 | 42 ✓ |
| `"9 8 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -"` | 42 | 42 ✓ |
| `"1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /"` | 15 | 15 ✓ |
| `"5 3 -"` | 2 | 2 ✓ |
| `"1 9 -"` | -8 | -8 ✓ |
| `"3 5 - 2 +"` | 0 | 0 ✓ |
| `"+"` | Error (stack < 2) | Error ✓ |
| `"5 +"` | Error (stack < 2) | Error ✓ |
| `"1 2 3"` | Error (stack size ≠ 1) | Error ✓ |
| `"5 0 /"` | Error (div by zero) | Error ✓ |
| `"12 3 +"` | Error (multi-char token) | Error ✓ |
| `"1 2 %"` | Error (unknown op) | Error ✓ |

---

## Exercise 02 — PmergeMe (Ford–Johnson Merge-Insert Sort)

### The problem in one sentence
> Sort a sequence of positive integers using the **Ford–Johnson algorithm** (merge-insert sort), implemented separately for `std::vector` and `std::deque`, with measured timing for each.

### Why `std::vector` and `std::deque`?

Both are **sequence containers** appropriate for sorting:

| Property | `std::vector` | `std::deque` |
|---|---|---|
| Memory layout | Contiguous single block | Segmented fixed-size chunks |
| Random access | O(1) — pointer arithmetic | O(1) — index calculation |
| Mid-insertion | O(N) — shift right | O(N) — shift within chunk |
| Cache locality | Excellent | Good (within chunks) |
| `lower_bound` | Works with RAI | Works with RAI |
| Typical sorting perf | Faster (cache) | Slightly slower |

The `std::deque` exists specifically to give a different container behavior on the same algorithm, which is what the evaluation sheet tests: **"explain the difference in time used for each container."**

> [!TIP]
> On most benchmarks, `std::vector` sorts faster than `std::deque` due to cache-friendly contiguous memory. The deque overhead comes from pointer indirection through its chunk table.

### Ford–Johnson algorithm — step by step

The Ford-Johnson (merge-insert) sort minimizes the number of comparisons required to sort N elements. Here is the full pipeline:

#### Step 1 — Pair the elements

```
Input:  [3, 5, 9, 7, 4, 2, 6]

Pairs:  (3,5)  (9,7)  (4,2)    ← leftover: 6
```

Each pair is locally sorted so the larger element is the **winner** and the smaller is the **loser**:

```
Normalized: (3→5)  (7→9)  (2→4)    leftover: 6
             small  small  small
             large  large  large
```

#### Step 2 — Recurse on the winners (main chain)

```
Winners: [5, 9, 4]  → sort recursively with Ford-Johnson
Sorted winners: [4, 5, 9]
```

The recursive call repeats the same pairing/sorting on a half-sized problem. This is the "merge" in merge-insert.

#### Step 3 — Build the initial main chain

```
After recursive sort of winners: [4, 5, 9]

The first winner's loser is inserted first (no comparisons needed because
  loser ≤ winner ≤ everything in main chain):

Initial chain: [2, 4, 5, 9]    ← 2 is loser of winner 4
Pending losers: [3, 7]          ← losers of winners 5, 9 respectively
```

#### Step 4 — Insert pending losers in Jacobsthal order

The Jacobsthal sequence governs the order in which pending elements are inserted:

```
Jacobsthal: 1, 3, 5, 11, 21, 43, 85, ...
           (each term = prev + 2 × prev-prev)
```

For k pending elements, indices are drawn from Jacobsthal groups to minimize worst-case comparisons. Within each group, indices count **downward**:

```
k=2 pending:  Jacobsthal picks index 1 (= min(3,2)), then index 2
Insert order: [2nd pending, 1st pending] → [7, 3] in this example
```

For each pending loser, binary search is bounded by the position of **its paired winner** in the main chain — the loser is guaranteed to be ≤ its winner, so it can only be inserted to the *left* of the winner.

```
Insert 7 (paired with winner 9 at pos 3):
  Binary search in [2, 4, 5, 9] up to pos of 9
  → 7 goes between 5 and 9: [2, 4, 5, 7, 9]

Insert 3 (paired with winner 5 at pos 2):
  Binary search in [2, 4, 5, 7, 9] up to pos of 5
  → 3 goes between 2 and 4: [2, 3, 4, 5, 7, 9]
```

#### Step 5 — Insert the straggler (odd element, if any)

```
Straggler: 6 (no bound constraint, binary search entire chain)
[2, 3, 4, 5, 6, 7, 9]  ✓
```

### Why the Jacobsthal insertion order matters

If you inserted pending elements in naive left-to-right order, each new element could require comparisons up to its current position in the main chain. Jacobsthal order ensures that **each new element reduces the upper bound for subsequent elements**, keeping the worst-case comparison count minimal.

```
Jacobsthal groups for k=5:
  Group 1: indices [3, 2, 1]  (from J₂=3 down to J₁=1+1=2)
  Group 2: indices [5, 4]     (from J₃=5 down to J₂+1=4)

This means we first insert the 3rd element (bound shrinks early for 1st, 2nd),
then 2nd, then 1st — optimal comparison budget.
```

### Element identity tracking

The `Element` struct carries an `id` field alongside `value`. During recursive sorting and pending-list construction, pairs must stay linked so that after winners are sorted, we can still match each winner to its loser.

```cpp
struct Element {
    size_t id;     // original index — survives all recursive sorts
    int    value;  // the number
    std::vector<Element> history; // loser stored here before recursion
};
```

When building the pending list from sorted winners, we retrieve each loser from `winner.history`. When doing bounded insertion, we find the winner's current position in the main chain by matching `id`.

### Why two separate implementations (not one template)?

The subject explicitly warns that using one generic function behind a template is discouraged. The point is to demonstrate understanding of **container-specific operations**:

- `std::vector` uses `push_back`, `pop_back`, index access, and iterator arithmetic.
- `std::deque` uses the same interface in name, but the underlying memory model, cache behavior, and reallocation mechanics are fundamentally different.

Writing both explicitly shows the evaluator that you understand that containers are not interchangeable boxes, but tools with distinct performance profiles.

### Output format

```
Before: 3 5 9 7 4 2 6
After:  2 3 4 5 6 7 9
Time to process a range of 7 elements with std::vector : 28.00000 us
Time to process a range of 7 elements with std::deque  : 29.00000 us
```

> [!IMPORTANT]
> Timing is measured with `gettimeofday` (C++98 compatible) and covers the **full sort including data management**, not just the inner loop. This matches the subject requirement.

### Validated edge cases

| Input | Expected | Result |
|---|---|---|
| Single element `42` | `Before: 42` / `After: 42` | ✓ |
| Two elements `9 3` | `After: 3 9` | ✓ |
| Already sorted `1 2 3 4 5 6 7` | unchanged order | ✓ |
| Reverse sorted `7 6 5 4 3 2 1` | fully sorted | ✓ |
| All same `3 3 3 3 3` | all same (stable) | ✓ |
| Odd-length 11 elements | sorted + straggler handled | ✓ |
| `+5` prefix | accepted as 5 | ✓ |
| `2147483648` (INT overflow) | `Error` | ✓ |
| Negative number `-3` | `Error` | ✓ |
| Zero `0` | `Error` (not positive) | ✓ |
| Float `2.5` | `Error` (non-integer) | ✓ |
| Non-numeric `abc` | `Error` | ✓ |
| 3000 unique integers | sorted correctly | ✓ |

---

## Complexity Summary

| Exercise | Data structure time complexity | Reason |
|---|---|---|
| ex00 load DB | O(N log N) for N CSV rows | `std::map` insertion |
| ex00 per query | O(log N) | `upper_bound` binary search on sorted map |
| ex01 evaluation | O(T) for T tokens | One pass, O(1) stack ops each |
| ex02 Ford-Johnson | O(N log N) comparisons | Minimized constant vs. merge sort |
| ex02 vector sort | O(N log N) with cache efficiency | Contiguous memory |
| ex02 deque sort | O(N log N) slightly higher constant | Chunk indirection overhead |

---

## Evaluator Q&A Cheat Sheet

**Q: Why did you use `std::map` in ex00?**
> Because the key operation is predecessor search — finding the closest earlier date. `std::map` keeps keys sorted automatically, and `upper_bound` gives me the next-greater key in O(log N). I then step back one position. A vector would require O(N) linear scan per query.

**Q: Why is the date string comparison valid without a custom comparator?**
> The date format `YYYY-MM-DD` has fixed-width fields, so lexicographic string order matches calendar order. No custom comparator is needed.

**Q: Why `std::list` in ex01 instead of `std::stack`?**
> `std::stack` is a container adaptor, not a container itself. `std::list` is a proper STL container that provides the same LIFO behavior through `push_back`/`pop_back`/`back`. It also hadn't been used in ex00, satisfying the no-reuse rule.

**Q: Why not implement Ford-Johnson as one template function in ex02?**
> The subject discourages it and evaluators expect proof of understanding. The internal operations — memory layout, reallocation, iterator category — differ between `vector` and `deque`. Writing two explicit implementations demonstrates that I understand them as distinct algorithmic tools, not just generic storage.

**Q: Why is the deque usually slower than the vector?**
> `std::deque` stores elements in fixed-size chunks linked by a pointer table. Accessing element `i` requires an indirect lookup through this table, breaking cache locality. `std::vector` stores everything in a single contiguous block — the CPU prefetcher handles it much more efficiently.

**Q: What happens if an element has no predecessor in the database?**
> `upper_bound` returns `begin()`. We check for this before decrementing, and print `Error: bad input => <date>`. This means the query date predates all historical data.

**Q: Why does `-0` get rejected?**
> IEEE 754 defines `-0.0 == 0.0`, so a simple `val < 0` check would accept `-0` as if it were `0`. We explicitly check whether the raw value string begins with `'-'` before parsing, which catches `-0` and rejects it as "not a positive number."
