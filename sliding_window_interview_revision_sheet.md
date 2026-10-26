# Sliding Window --- DSA Interview Revision Sheet (C++)

**Scope:** interview-focused sliding-window concepts and famous LeetCode
Easy/Medium problems. Each main problem includes C++ code and a
four-step explanation. The central skill is recognising when a
contiguous range can be maintained incrementally instead of
recalculating it from scratch.

------------------------------------------------------------------------

# Part A --- Core Concepts

## 1. What is a sliding window?

A **window** is a contiguous range of an array/string, commonly
represented by two pointers `left` and `right`.

Instead of examining every possible subarray from scratch, we: 1. Expand
the window by moving `right`. 2. Update information about the new
element. 3. If the window violates a condition, move `left` to shrink
it. 4. Update the answer whenever the window satisfies the problem's
requirement.

Two broad types: - **Fixed-size window:** window length is exactly
`k`. - **Variable-size window:** expand/shrink according to a
constraint, such as at most `k` distinct values or sum/length
conditions.

A subarray or substring must be **contiguous**. A subsequence does not
have to be contiguous.

## 2. Fixed-size window template

Use this when the problem asks about every contiguous range of exactly
`k` elements, such as maximum sum of any length-`k` subarray.

``` cpp
long long fixedWindowMaxSum(vector<int>& nums, int k) {
    int n = nums.size();
    if (k <= 0 || k > n) return 0; // adapt to problem constraints

    long long windowSum = 0;
    for (int i = 0; i < k; ++i) windowSum += nums[i];

    long long best = windowSum;
    for (int right = k; right < n; ++right) {
        windowSum += nums[right];       // include new right item
        windowSum -= nums[right - k];   // remove item leaving left
        best = max(best, windowSum);
    }
    return best;
}
```

**Why it works:** when the window shifts one step, only one element
enters and one element leaves. Recomputing each window would cost
`O(nk)`; updating incrementally costs `O(n)`.

## 3. Variable-size window template

Common pattern for constraints where a window becomes invalid as it
grows, and removing items from the left can restore validity.

``` cpp
int longestValidWindow(vector<int>& nums) {
    int left = 0, best = 0;
    // Maintain any state required by the problem.
    for (int right = 0; right < (int)nums.size(); ++right) {
        // Add nums[right] to window state.

        while (/* window is invalid */) {
            // Remove nums[left] from state.
            ++left;
        }

        best = max(best, right - left + 1);
    }
    return best;
}
```

For longest valid window, update `best` after shrinking until valid. For
shortest valid window, update the answer while the condition is
satisfied, then shrink as far as possible.

## 4. Frequency-map window template for strings

``` cpp
int left = 0, best = 0;
unordered_map<char, int> freq;

for (int right = 0; right < (int)s.size(); ++right) {
    ++freq[s[right]];                 // add right character

    while (/* constraint is violated */) {
        if (--freq[s[left]] == 0) freq.erase(s[left]);
        ++left;
    }

    best = max(best, right - left + 1);
}
```

Use an array of 26 or 128/256 counters instead of a hash map when the
character set is known and small.

## 5. How to recognise sliding-window problems

Look for phrases like: - longest/shortest **substring** or **subarray**
satisfying a condition; - maximum/minimum sum over a range of fixed size
`k`; - at most `k` distinct characters/elements; - no repeated
characters; - replace at most `k` characters; - find an
anagram/permutation inside a string; - count windows satisfying a
frequency requirement.

Sliding window is most straightforward when the window is contiguous and
its state can be updated cheaply. For a sum constraint with **negative
numbers**, the usual "expand right, shrink while sum too large" logic
may fail because adding a negative value can reduce the sum. Consider
prefix sums + hash maps or other techniques in that case.

## 6. Common sliding-window patterns

  ---------------------------------------------------------------------------
  Pattern                 Typical question        Core technique
  ----------------------- ----------------------- ---------------------------
  Fixed size              Max sum/average of `k`  Add right, subtract leaving
                          items                   left

  Longest unique          No repeated characters  Frequency/last-seen index;
  substring                                       shrink until valid

  At most K               At most `k` distinct    Frequency map +
                          values                  `while (distinct > k)`

  Exactly K               Exactly `k` distinct    `atMost(k) - atMost(k-1)`
                          values                  for counts

  Minimum valid window    Smallest substring      Expand to valid, record,
                          covering requirements   then shrink

  Character replacement   At most `k`             Window length minus max
                          replacements            frequency

  Anagram/permutation     Match character counts  Fixed-length window +
                                                  frequency counts

  Count valid subarrays   Number of windows       Add `right-left+1` when all
                          satisfying monotonic    suffixes ending at `right`
                          condition               are valid
  ---------------------------------------------------------------------------

------------------------------------------------------------------------

# Part B --- Fixed-Size Sliding Window Problems

## 1. Maximum Sum of a Subarray of Size K (classic pattern)

This is a standard pattern problem; it does not have one single
canonical LeetCode ID.

``` cpp
long long maxSumSizeK(vector<int>& nums, int k) {
    int n = nums.size();
    if (k <= 0 || k > n) return 0;
    long long sum = 0;
    for (int i = 0; i < k; ++i) sum += nums[i];
    long long best = sum;

    for (int r = k; r < n; ++r) {
        sum += nums[r] - nums[r - k];
        best = max(best, sum);
    }
    return best;
}
```

**4-step approach** 1. Calculate the sum of the first `k` elements. 2.
Store it as the current best. 3. Move the window: add the new right
element and subtract the element leaving on the left. 4. Update the best
after each shift. Time `O(n)`, space `O(1)`.

## 2. LeetCode 643 --- Maximum Average Subarray I (Easy)

``` cpp
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        long long sum = 0;
        for (int i = 0; i < k; ++i) sum += nums[i];
        long long best = sum;

        for (int r = k; r < (int)nums.size(); ++r) {
            sum += nums[r] - nums[r - k];
            best = max(best, sum);
        }
        return (double)best / k;
    }
};
```

**4-step approach** 1. Sum the first `k` values. 2. Slide the fixed
window by adding the incoming value and subtracting the outgoing value.
3. Keep the maximum window sum. 4. Divide the maximum sum by `k`. Time
`O(n)`, space `O(1)`.

## 3. LeetCode 1456 --- Maximum Number of Vowels in a Substring of Given Length (Medium)

``` cpp
class Solution {
    bool vowel(char c) {
        return c == 'a' || c == 'e' || c == 'i' ||
               c == 'o' || c == 'u';
    }
public:
    int maxVowels(string s, int k) {
        int count = 0, best = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (vowel(s[i])) ++count;
            if (i >= k && vowel(s[i - k])) --count;
            if (i >= k - 1) best = max(best, count);
        }
        return best;
    }
};
```

**4-step approach** 1. Keep the number of vowels in the current
length-`k` window. 2. Add one if the incoming character is a vowel. 3.
Once the window exceeds `k`, remove the outgoing character's
contribution. 4. Update the answer once a full window exists. Time
`O(n)`, space `O(1)`.

## 4. LeetCode 1343 --- Number of Sub-arrays of Size K and Average Greater than or Equal to Threshold (Medium)

``` cpp
class Solution {
public:
    int numOfSubarrays(vector<int>& arr, int k, int threshold) {
        long long sum = 0;
        int ans = 0;
        for (int i = 0; i < (int)arr.size(); ++i) {
            sum += arr[i];
            if (i >= k) sum -= arr[i - k];
            if (i >= k - 1 && sum >= 1LL * k * threshold) ++ans;
        }
        return ans;
    }
};
```

**4-step approach** 1. Maintain the sum of the most recent `k` elements.
2. Add each incoming value and remove the value that is now outside the
window. 3. Once the window contains `k` elements, compare its sum with
`k * threshold`. 4. Increment the count for every qualifying window.
Time `O(n)`, space `O(1)`.

------------------------------------------------------------------------

# Part C --- Variable-Size Windows: Longest Valid Window

## 1. LeetCode 3 --- Longest Substring Without Repeating Characters (Medium)

``` cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        vector<int> last(256, -1);
        int left = 0, best = 0;

        for (int right = 0; right < (int)s.size(); ++right) {
            unsigned char c = s[right];
            if (last[c] >= left) left = last[c] + 1;
            last[c] = right;
            best = max(best, right - left + 1);
        }
        return best;
    }
};
```

**4-step approach** 1. Store the last index at which each character was
seen. 2. If the current character's previous index is inside the window,
move `left` past it. 3. Update that character's last-seen index to
`right`. 4. Update the longest valid length. Time `O(n)`, space `O(1)`
for the fixed byte alphabet.

## 2. LeetCode 209 --- Minimum Size Subarray Sum (Medium)

**Important:** this standard sliding-window solution assumes all array
values are positive, as guaranteed by the problem.

``` cpp
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int left = 0, best = INT_MAX;
        long long sum = 0;

        for (int right = 0; right < (int)nums.size(); ++right) {
            sum += nums[right];
            while (sum >= target) {
                best = min(best, right - left + 1);
                sum -= nums[left++];
            }
        }
        return best == INT_MAX ? 0 : best;
    }
};
```

**4-step approach** 1. Expand the right pointer and add its value to the
sum. 2. While the sum reaches the target, record the current length. 3.
Remove the leftmost value and advance `left` to seek a shorter valid
window. 4. Return the minimum length, or `0` if no window qualifies.
Time `O(n)`, space `O(1)`.

## 3. LeetCode 1004 --- Max Consecutive Ones III (Medium)

``` cpp
class Solution {
public:
    int longestOnes(vector<int>& nums, int k) {
        int left = 0, zeros = 0, best = 0;
        for (int right = 0; right < (int)nums.size(); ++right) {
            if (nums[right] == 0) ++zeros;
            while (zeros > k) {
                if (nums[left] == 0) --zeros;
                ++left;
            }
            best = max(best, right - left + 1);
        }
        return best;
    }
};
```

**4-step approach** 1. Expand the window and count zeros. 2. If zeros
exceed `k`, move `left` until at most `k` zeros remain. 3. The valid
window represents a range where at most `k` zeros need flipping. 4.
Track the maximum length. Time `O(n)`, space `O(1)`.

## 4. LeetCode 1493 --- Longest Subarray of 1's After Deleting One Element (Medium)

``` cpp
class Solution {
public:
    int longestSubarray(vector<int>& nums) {
        int left = 0, zeros = 0, best = 0;
        for (int right = 0; right < (int)nums.size(); ++right) {
            if (nums[right] == 0) ++zeros;
            while (zeros > 1) {
                if (nums[left] == 0) --zeros;
                ++left;
            }
            best = max(best, right - left); // must delete exactly one element
        }
        return best;
    }
};
```

**4-step approach** 1. Maintain a window with at most one zero. 2.
Shrink from the left while the window contains more than one zero. 3.
Since one element must be deleted, candidate length is `right - left`,
not the full window length. 4. Keep the maximum candidate. Time `O(n)`,
space `O(1)`.

## 5. LeetCode 904 --- Fruit Into Baskets (Medium)

Equivalent to finding the longest subarray containing at most two
distinct values.

``` cpp
class Solution {
public:
    int totalFruit(vector<int>& fruits) {
        unordered_map<int, int> freq;
        int left = 0, best = 0;
        for (int right = 0; right < (int)fruits.size(); ++right) {
            ++freq[fruits[right]];
            while (freq.size() > 2) {
                if (--freq[fruits[left]] == 0) freq.erase(fruits[left]);
                ++left;
            }
            best = max(best, right - left + 1);
        }
        return best;
    }
};
```

**4-step approach** 1. Add the incoming fruit to a frequency map. 2.
While there are more than two distinct fruit types, remove items from
the left. 3. Erase a type when its frequency reaches zero. 4. Update the
longest valid window. Time `O(n)` average, space `O(1)` for at most
three map keys at a time.

## 6. At most K distinct characters/elements --- reusable template

``` cpp
int longestAtMostKDistinct(string s, int k) {
    if (k == 0) return 0;
    unordered_map<char, int> freq;
    int left = 0, best = 0;

    for (int right = 0; right < (int)s.size(); ++right) {
        ++freq[s[right]];
        while ((int)freq.size() > k) {
            if (--freq[s[left]] == 0) freq.erase(s[left]);
            ++left;
        }
        best = max(best, right - left + 1);
    }
    return best;
}
```

**4-step approach** 1. Add the right element to its frequency count. 2.
The number of distinct values equals the number of keys in the map. 3.
Shrink left until the distinct count is at most `k`. 4. Update the
maximum valid length. Time `O(n)` average.

------------------------------------------------------------------------

# Part D --- Frequency Matching, Anagrams, and Replacements

## 1. LeetCode 567 --- Permutation in String (Medium)

``` cpp
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        if (s1.size() > s2.size()) return false;
        vector<int> need(26, 0), win(26, 0);
        for (char c : s1) ++need[c - 'a'];

        int k = s1.size();
        for (int r = 0; r < (int)s2.size(); ++r) {
            ++win[s2[r] - 'a'];
            if (r >= k) --win[s2[r - k] - 'a'];
            if (r >= k - 1 && win == need) return true;
        }
        return false;
    }
};
```

**4-step approach** 1. Count character frequencies in `s1`; any
permutation must have exactly those counts. 2. Maintain a window of
length `s1.size()` in `s2`. 3. Add the new character and remove the
outgoing character when the window is too long. 4. If the two frequency
arrays match, a permutation exists. Time `O(n + 26m)` conceptually
(array comparison is 26), space `O(1)`.

## 2. LeetCode 438 --- Find All Anagrams in a String (Medium)

``` cpp
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> ans;
        if (p.size() > s.size()) return ans;
        vector<int> need(26, 0), win(26, 0);
        for (char c : p) ++need[c - 'a'];

        int k = p.size();
        for (int r = 0; r < (int)s.size(); ++r) {
            ++win[s[r] - 'a'];
            if (r >= k) --win[s[r - k] - 'a'];
            if (r >= k - 1 && win == need) ans.push_back(r - k + 1);
        }
        return ans;
    }
};
```

**4-step approach** 1. Count the required character frequencies of `p`.
2. Slide a fixed-size window of length `p.size()` across `s`. 3. Keep
the window's character counts updated as characters enter and leave. 4.
Whenever counts match, add the window's starting index. Time `O(n*26)`
with array comparisons, effectively `O(n)`; space `O(1)`.

## 3. LeetCode 424 --- Longest Repeating Character Replacement (Medium)

``` cpp
class Solution {
public:
    int characterReplacement(string s, int k) {
        vector<int> freq(26, 0);
        int left = 0, maxFreq = 0, best = 0;

        for (int right = 0; right < (int)s.size(); ++right) {
            maxFreq = max(maxFreq, ++freq[s[right] - 'A']);
            while ((right - left + 1) - maxFreq > k) {
                --freq[s[left] - 'A'];
                ++left;
            }
            best = max(best, right - left + 1);
        }
        return best;
    }
};
```

**4-step approach** 1. Count each character and maintain the highest
frequency seen in the current/expanded window. 2. Window length minus
`maxFreq` estimates the replacements needed to make the window uniform.
3. If replacements needed exceed `k`, shrink from the left. 4. Track the
largest valid length. Time `O(n)`, space `O(1)`.

**Subtlety:** the common efficient solution does not decrease `maxFreq`
when shrinking. This stale maximum is intentional and still gives the
correct maximum length; it is not a precise current-window maximum at
every step.

## 4. LeetCode 76 --- Minimum Window Substring (Hard --- optional)

This is a famous pattern but **Hard**, included only as an optional
extension after Medium problems.

``` cpp
class Solution {
public:
    string minWindow(string s, string t) {
        if (t.empty() || s.size() < t.size()) return "";
        vector<int> need(128, 0);
        for (unsigned char c : t) ++need[c];

        int missing = t.size(), left = 0, bestStart = 0, bestLen = INT_MAX;
        for (int right = 0; right < (int)s.size(); ++right) {
            unsigned char c = s[right];
            if (need[c] > 0) --missing;
            --need[c];

            while (missing == 0) {
                int len = right - left + 1;
                if (len < bestLen) {
                    bestLen = len;
                    bestStart = left;
                }
                unsigned char out = s[left++];
                ++need[out];
                if (need[out] > 0) ++missing;
            }
        }
        return bestLen == INT_MAX ? "" : s.substr(bestStart, bestLen);
    }
};
```

**4-step approach** 1. Count required characters; `missing` tracks how
many required occurrences are still absent. 2. Expand right and update
counts until every required occurrence is included. 3. While valid,
record the shortest window, then remove from the left to try to make it
smaller. 4. Return the shortest valid substring, or empty string if none
exists. Time `O(|s|+|t|)`, space `O(ASCII)`.

------------------------------------------------------------------------

# Part E --- Counting Subarrays with Sliding Windows

## 1. Exactly K distinct integers --- LeetCode 992 (Hard; optional concept)

The standard identity is:
`countExactlyK(k) = countAtMostK(k) - countAtMostK(k - 1)`. This is
useful when counting windows with exactly `k` distinct values. The same
identity is commonly used for substring counts. Because LeetCode 992 is
Hard, leave it until the core Easy/Medium patterns are comfortable.

## 2. LeetCode 1358 --- Number of Substrings Containing All Three Characters (Medium)

``` cpp
class Solution {
public:
    int numberOfSubstrings(string s) {
        int last[3] = {-1, -1, -1};
        int ans = 0;
        for (int right = 0; right < (int)s.size(); ++right) {
            last[s[right] - 'a'] = right;
            ans += 1 + min(last[0], min(last[1], last[2]));
        }
        return ans;
    }
};
```

**4-step approach** 1. Store the most recent index of each of `'a'`,
`'b'`, and `'c'`. 2. At each right index, update the last-seen position
for the current character. 3. If all three have appeared, the minimum of
their last-seen indices is the last possible start that includes all
three. 4. Add `min(lastA,lastB,lastC) + 1` to the answer; if any is
`-1`, this contributes zero. Time `O(n)`, space `O(1)`.

## 3. Counting subarrays with all positive values and product less than K --- LeetCode 713 (Medium)

``` cpp
class Solution {
public:
    int numSubarrayProductLessThanK(vector<int>& nums, int k) {
        if (k <= 1) return 0;
        long long product = 1;
        int left = 0, ans = 0;
        for (int right = 0; right < (int)nums.size(); ++right) {
            product *= nums[right];
            while (product >= k) product /= nums[left++];
            ans += right - left + 1;
        }
        return ans;
    }
};
```

**4-step approach** 1. Since every number is positive, expand the window
and multiply by the new value. 2. While the product is at least `k`,
divide out leftmost values and advance `left`. 3. The window is now
valid; every suffix ending at `right` is also valid. 4. Add
`right-left+1` subarrays to the answer. Time `O(n)`, space `O(1)`;
problem values are positive.

------------------------------------------------------------------------

# Part F --- Common Mistakes and Interview Traps

1.  **Use `while`, not always `if`:** a window may need multiple left
    moves to become valid again.
2.  **Update answer at the right time:** longest valid window is usually
    updated after shrinking; minimum valid window is updated while valid
    and before removing the left item.
3.  **Correct outgoing index for fixed size:** when adding
    `nums[right]`, remove `nums[right-k]` once `right >= k`.
4.  **Count length correctly:** inclusive window length is
    `right-left+1`.
5.  **Frequency cleanup:** erase a map key when its count reaches zero;
    otherwise `freq.size()` overcounts distinct values.
6.  **Do not confuse substring with subsequence:** sliding window
    applies to contiguous ranges.
7.  **Negative values:** ordinary sum-based expand/shrink logic often
    fails with negative numbers.
8.  **Fixed alphabet:** `vector<int>(26)` assumes lowercase English
    letters; use an appropriate structure for other character sets.
9.  **Empty input / invalid k:** handle according to problem
    constraints; avoid indexing outside the array.
10. **Integer overflow:** use `long long` for large sums/products where
    constraints require it.
11. **Exact K vs at most K:** for counts, exact K is often
    `atMost(K) - atMost(K-1)`, not a simple single-window condition.
12. **Stale max frequency:** in Longest Repeating Character Replacement,
    `maxFreq` is intentionally not reduced during shrinking in the
    standard optimized solution.

------------------------------------------------------------------------

# Part G --- Complexity Cheat Sheet

  -------------------------------------------------------------------------
  Pattern                            Typical time               Extra space
  --------------------- ------------------------- -------------------------
  Fixed-size                               `O(n)`                    `O(1)`
  sum/average                                     

  Unique substring                         `O(n)`     `O(1)` fixed alphabet
  using last-seen array                           

  At most K distinct               `O(n)` average                    `O(k)`
  using map                                       

  Anagram/permutation           `O(26n)` = `O(n)`                    `O(1)`
  frequency array                                 

  Character replacement                    `O(n)`                    `O(1)`

  Minimum size                             `O(n)`                    `O(1)`
  positive-sum subarray                           

  Product less than K                      `O(n)`                    `O(1)`
  (positive values)                               

  Minimum window                     `O(|s|+|t|)`                `O(ASCII)`
  substring                                       
  -------------------------------------------------------------------------

Each pointer moves forward at most `n` times, which is why a correctly
implemented sliding window is usually linear time.

------------------------------------------------------------------------

# Part H --- Famous Easy/Medium Problems to Practise

  ------------------------------------------------------------------------
  LeetCode         Problem                     Difficulty Pattern
  ---------------- ---------------- --------------------- ----------------
  643              Maximum Average                   Easy Fixed window
                   Subarray I                             

  1456             Maximum Number                  Medium Fixed window
                   of Vowels in a                         
                   Substring of                           
                   Given Length                           

  1343             Number of                       Medium Fixed window
                   Sub-arrays of                          
                   Size K and                             
                   Average at Least                       
                   Threshold                              

  3                Longest                         Medium Last-seen index
                   Substring                              
                   Without                                
                   Repeating                              
                   Characters                             

  209              Minimum Size                    Medium Minimum valid
                   Subarray Sum                           window

  1004             Max Consecutive                 Medium At most K zeros
                   Ones III                               

  1493             Longest Subarray                Medium At most one zero
                   of 1's After                           
                   Deleting One                           
                   Element                                

  904              Fruit Into                      Medium At most two
                   Baskets                                distinct

  567              Permutation in                  Medium Fixed frequency
                   String                                 window

  438              Find All                        Medium Fixed frequency
                   Anagrams in a                          window
                   String                                 

  424              Longest                         Medium Frequency +
                   Repeating                              replacements
                   Character                              
                   Replacement                            

  1358             Number of                       Medium Last-seen
                   Substrings                             counting
                   Containing All                         
                   Three Characters                       

  713              Subarray Product                Medium Positive product
                   Less Than K                            window

  76               Minimum Window         Hard (optional) Minimum valid
                   Substring                              frequency window

  992              Subarrays with K       Hard (optional) AtMost
                   Different                              difference
                   Integers                               
  ------------------------------------------------------------------------

### Recommended practice order

1.  Fixed window: **643 → 1456 → 1343**.
2.  Longest valid window: **3 → 1004 → 904**.
3.  Minimum valid window: **209**.
4.  Frequency matching: **567 → 438**.
5.  Frequency/replacement logic: **424**.
6.  Counting valid windows: **713 → 1358**.
7.  Optional Hard extensions: **76 → 992**.

------------------------------------------------------------------------

# Part I --- Mock Interview Checklist

Before coding any sliding-window problem, answer: 1. Is the window
fixed-size or variable-size? 2. What exact condition makes the window
valid/invalid? 3. What state must be updated when an element enters or
leaves? 4. Should the answer be updated before or after shrinking? 5.
Can negative numbers invalidate the greedy shrinking logic? 6. Is the
question asking for a longest window, shortest window, or number of
windows? 7. What does `right - left + 1` represent here? 8. Why is the
algorithm `O(n)` even though there is a nested `while` loop?

**Interview explanation:** "Each element enters the window once and
leaves at most once. Both pointers move only forward, so the total
pointer operations are linear, giving `O(n)` time."
