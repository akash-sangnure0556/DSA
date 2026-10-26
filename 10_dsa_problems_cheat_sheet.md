# NetApp MTS2 — 10 DSA Problems Cheat Sheet
## Easy-to-reconstruct, optimal C++ solutions + interview explanations

Use this as a revision sheet. For each problem, learn the key observation, the invariant, the code, and the complexity. Try to code it independently before looking at the solution.

---

## 1. Search in Rotated Sorted Array — LeetCode 33 (Medium)

### Core idea
At every step, at least one half of the current range is sorted. Identify the sorted half and check whether the target lies inside it. Discard the other half.

### Optimal C++ solution — O(log n)
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = (int)nums.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] == target) return mid;

            // Left half is sorted.
            if (nums[left] <= nums[mid]) {
                if (nums[left] <= target && target < nums[mid]) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
            // Otherwise, right half is sorted.
            else {
                if (nums[mid] < target && target <= nums[right]) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }

        return -1;
    }
};
```

### Four-step explanation
1. Set `left` and `right` to the current search range.
2. Calculate `mid`; return immediately if the target is there.
3. Check which half is sorted. If the target falls within that sorted half's boundaries, search there.
4. Otherwise, discard that half and continue in the other half.

### Complexity
- Time: O(log n).
- Extra space: O(1).

### Common mistakes
- Use `left <= right`, because a single remaining element must be checked.
- The standard problem assumes distinct values. Duplicates need extra handling.
- Use `left + (right - left) / 2` to avoid midpoint overflow.

---

## 2. Reverse Linked List — LeetCode 206 (Easy)

### Core idea
Reverse each `next` pointer. Keep track of the previous node, current node, and next node so the rest of the list is not lost.

### Optimal C++ solution — O(n), O(1) extra space
```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;

        while (curr != nullptr) {
            ListNode* nextNode = curr->next; // save next node
            curr->next = prev;               // reverse pointer
            prev = curr;                      // advance prev
            curr = nextNode;                  // advance curr
        }

        return prev;
    }
};
```

### Four-step explanation
1. Start `prev` at `nullptr` and `curr` at the head.
2. Save `curr->next` before changing any pointer.
3. Point `curr->next` backward to `prev`, then move both pointers forward.
4. When `curr` becomes null, `prev` points to the new head.

### Complexity
- Time: O(n).
- Extra space: O(1).

### Common mistakes
- Save `nextNode` before reversing the link.
- Return `prev`, not `head`.
- Empty and one-node lists work without special cases.

---

## 3. Linked List Cycle Detection — LeetCode 141 (Easy)

### Core idea
Use Floyd's slow-and-fast pointer algorithm. `slow` advances one node at a time and `fast` advances two. If a cycle exists, they eventually meet.

### Optimal C++ solution — O(n), O(1) extra space
```cpp
class Solution {
public:
    bool hasCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;

            if (slow == fast) return true;
        }

        return false;
    }
};
```

### Four-step explanation
1. Initialize both pointers at the head.
2. Move `slow` one step and `fast` two steps while both fast-pointer links are valid.
3. If they meet, a cycle exists.
4. If `fast` reaches null, the list ends, so there is no cycle.

### Complexity
- Time: O(n).
- Extra space: O(1).

### Common mistakes
- Check `fast != nullptr && fast->next != nullptr` before moving two steps.
- Compare node pointers (`slow == fast`), not node values.
- For cycle entry, use the related problem LeetCode 142.

---

## 4. Binary Tree Level Order Traversal — LeetCode 102 (Medium)

### Core idea
Breadth-first search (BFS) visits nodes level by level. At the start of each iteration, capture the queue's size: that is the number of nodes in the current level.

### Optimal C++ solution — O(n)
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (root == nullptr) return result;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = (int)q.size();
            vector<int> level;

            for (int i = 0; i < levelSize; ++i) {
                TreeNode* node = q.front();
                q.pop();

                level.push_back(node->val);

                if (node->left != nullptr) q.push(node->left);
                if (node->right != nullptr) q.push(node->right);
            }

            result.push_back(level);
        }

        return result;
    }
};
```

### Four-step explanation
1. If the root is null, return an empty result.
2. Push the root into a queue.
3. For each level, record the queue size and pop exactly that many nodes; add their values to the current level.
4. Push each popped node's children and append the completed level to the result.

### Complexity
- Time: O(n), because every node is processed once.
- Space: O(n) for the queue and result in the worst case.

### Common mistakes
- Capture `levelSize` before processing the level; do not use a changing queue size as the loop limit.
- Push only non-null children.
- This is BFS, not DFS.

---

## 5. Trapping Rain Water — LeetCode 42 (Hard)

### Core idea
Water above index `i` is determined by the shorter of the tallest walls to its left and right, minus the current height. The optimal two-pointer approach processes the side whose current height is smaller.

### Optimal two-pointer C++ solution — O(n), O(1) extra space
```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int left = 0, right = (int)height.size() - 1;
        int leftMax = 0, rightMax = 0;
        int water = 0;

        while (left < right) {
            if (height[left] <= height[right]) {
                if (height[left] >= leftMax) {
                    leftMax = height[left];
                } else {
                    water += leftMax - height[left];
                }
                ++left;
            } else {
                if (height[right] >= rightMax) {
                    rightMax = height[right];
                } else {
                    water += rightMax - height[right];
                }
                --right;
            }
        }

        return water;
    }
};
```

### Four-step explanation
1. Place pointers at both ends; maintain the highest wall seen from each side.
2. Compare `height[left]` and `height[right]`.
3. Process the shorter side: its water level is safely bounded by the maximum seen from that side, because the opposite boundary is at least as tall as the current side.
4. Update that side's maximum or add the difference between its maximum and current height, then move that pointer inward.

### Complexity
- Time: O(n).
- Extra space: O(1).

### Remember
- Water is trapped above a bar only when there is a higher boundary on both sides.
- The per-index formula is `min(leftMax, rightMax) - height[i]`, when positive.
- In the two-pointer algorithm, **process the side with the smaller current height**. Do not blindly process left first.

---

## 6. Longest Common Prefix — LeetCode 14 (Easy)

### Core idea
Start with the first string as the candidate prefix. Compare it against every other string and shorten it until it matches the beginning of that string.

### Simple optimal-enough C++ solution
```cpp
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";

        string prefix = strs[0];

        for (int i = 1; i < (int)strs.size(); ++i) {
            while (strs[i].compare(0, prefix.size(), prefix) != 0) {
                prefix.pop_back();
                if (prefix.empty()) return "";
            }
        }

        return prefix;
    }
};
```

### Four-step explanation
1. If the list is empty, return an empty string.
2. Assume the first string is the common prefix.
3. For each next string, keep shortening the prefix until it matches that string's beginning.
4. If the prefix becomes empty, no non-empty common prefix exists; otherwise return it after all strings are checked.

### Complexity
Let `S` be the total number of characters across all strings.
- Time: O(S) in the standard bound.
- Extra space: O(1) beyond the returned prefix (ignoring string storage details).

### Common mistakes
- A prefix must start at index zero in every string.
- One string may itself be the shortest prefix.
- Empty input should return `""`.

---

## 7. Longest Palindromic Substring — LeetCode 5 (Medium)

### Core idea
Every palindrome has a center. Expand outward from every possible center. There are `n` odd-length centers and `n - 1` even-length centers.

### Easy-to-understand optimal-practical C++ solution — O(n²), O(1) extra space
```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        int n = (int)s.size();
        int bestStart = 0, bestLen = 0;

        auto expand = [&](int left, int right) {
            while (left >= 0 && right < n && s[left] == s[right]) {
                --left;
                ++right;
            }

            int len = right - left - 1;
            if (len > bestLen) {
                bestLen = len;
                bestStart = left + 1;
            }
        };

        for (int center = 0; center < n; ++center) {
            expand(center, center);     // odd length
            expand(center, center + 1); // even length
        }

        return s.substr(bestStart, bestLen);
    }
};
```

### Four-step explanation
1. Treat every character as an odd-length palindrome center.
2. Also treat every gap between adjacent characters as an even-length center.
3. Expand left and right while the characters match and the indices stay valid.
4. Whenever the expanded palindrome is longer than the best so far, save its starting index and length; return that substring.

### Complexity
- Time: O(n²) worst case.
- Extra space: O(1), excluding the returned string.

### Common mistakes
- Check both odd and even centers; missing even centers fails on strings like `"cbbd"`.
- After expansion, the valid interval is `left + 1` through `right - 1`.
- Manacher's algorithm is O(n), but center expansion is usually simpler and interview-friendly.

---

## 8. Partition Labels — LeetCode 763 (Medium)

### Core idea
Each character must appear in only one partition. First record the last index of each character. While scanning, extend the current partition to include the last occurrence of every character seen so far.

### Optimal greedy C++ solution — O(n)
```cpp
class Solution {
public:
    vector<int> partitionLabels(string s) {
        vector<int> last(26, -1);

        for (int i = 0; i < (int)s.size(); ++i) {
            last[s[i] - 'a'] = i;
        }

        vector<int> answer;
        int start = 0, end = 0;

        for (int i = 0; i < (int)s.size(); ++i) {
            end = max(end, last[s[i] - 'a']);

            if (i == end) {
                answer.push_back(end - start + 1);
                start = i + 1;
            }
        }

        return answer;
    }
};
```

### Four-step explanation
1. Precompute the last occurrence of every character.
2. Scan the string while tracking the farthest last occurrence (`end`) required by the current partition.
3. If the current index reaches `end`, every character in this partition has its final occurrence inside it.
4. Record the partition length and start a new partition.

### Complexity
- Time: O(n).
- Extra space: O(1), because the alphabet has 26 lowercase letters.

### Common mistakes
- Update `end` with `max`; replacing it with the current character's last index can shrink the boundary incorrectly.
- A partition ends only when `i == end`.
- The standard problem assumes lowercase English letters.

---

## 9. Topological Sort — Kahn's Algorithm (BFS)

### Core idea
A topological order is possible only for a directed acyclic graph (DAG). Compute indegrees; start with all nodes having indegree zero. Remove them one by one and reduce their neighbors' indegrees.

### C++ solution
```cpp
vector<int> topologicalSort(int V, vector<vector<int>>& adj) {
    vector<int> indegree(V, 0);

    for (int u = 0; u < V; ++u) {
        for (int v : adj[u]) {
            ++indegree[v];
        }
    }

    queue<int> q;
    for (int node = 0; node < V; ++node) {
        if (indegree[node] == 0) q.push(node);
    }

    vector<int> order;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);

        for (int v : adj[u]) {
            --indegree[v];
            if (indegree[v] == 0) q.push(v);
        }
    }

    // If order.size() < V, the graph contains a directed cycle.
    return order;
}
```

### Four-step explanation
1. Calculate each node's indegree, the number of incoming edges.
2. Put every zero-indegree node into a queue.
3. Pop a node, append it to the order, and decrement the indegree of each outgoing neighbor; enqueue a neighbor when its indegree becomes zero.
4. If the order contains all `V` nodes, it is a valid topological ordering; otherwise a directed cycle exists.

### Complexity
- Time: O(V + E).
- Extra space: O(V).

### Important notes
- Topological sorting is for directed graphs.
- More than one valid order may exist.
- For a cycle-aware API, return an empty result or a pair `{order, isPossible}` if `order.size() != V`.
- For LeetCode Course Schedule II (210), return `{}` when a cycle exists.

---

## 10. Binary Search — exact match (Easy)

### Core idea
The array must be sorted. At each step, compare the target with the middle element and discard half of the remaining range.

### Optimal iterative C++ solution — O(log n)
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = (int)nums.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] == target) return mid;
            if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        return -1;
    }
};
```

### Four-step explanation
1. Initialize the inclusive search range from index `0` to `n - 1`.
2. Find the middle index without adding `left + right` directly.
3. If the middle value matches, return its index; if it is too small, search the right half, otherwise search the left half.
4. If `left > right`, the target is absent, so return `-1`.

### Complexity
- Time: O(log n).
- Extra space: O(1).

### Common mistakes
- The input must be sorted.
- With an inclusive range, use `while (left <= right)`.
- Move past `mid` with `mid + 1` or `mid - 1` to ensure progress.

---

# One-page pattern recap

| Problem | Pattern | Key invariant / observation | Time |
|---|---|---|---|
| Search in Rotated Sorted Array | Modified binary search | At least one half is sorted | O(log n) |
| Reverse Linked List | Pointer manipulation | `prev` is reversed prefix; `curr` is unprocessed suffix | O(n) |
| Linked List Cycle | Fast/slow pointers | Fast moves twice as quickly | O(n) |
| Level Order Traversal | BFS + queue | Capture queue size at start of each level | O(n) |
| Trapping Rain Water | Two pointers | Process the side with smaller current height | O(n) |
| Longest Common Prefix | Prefix shrinking | Candidate must prefix every processed string | O(total characters) |
| Longest Palindromic Substring | Expand around center | Try odd and even centers | O(n²) |
| Partition Labels | Greedy + last occurrence | Current partition ends at farthest required last index | O(n) |
| Topological Sort | Kahn's BFS | Process nodes only when indegree becomes zero | O(V + E) |
| Binary Search | Divide and conquer | Discard half every iteration | O(log n) |

# Recommended practice order

1. Binary Search
2. Reverse Linked List
3. Linked List Cycle Detection
4. Longest Common Prefix
5. Binary Tree Level Order Traversal
6. Search in Rotated Sorted Array
7. Partition Labels
8. Longest Palindromic Substring
9. Trapping Rain Water
10. Topological Sort

# Interview drill

For each problem, practise saying:
1. **Observation:** What property makes the approach work?
2. **Invariant:** What remains true after every iteration?
3. **Complexity:** Why is the time/space bound correct?
4. **Edge cases:** What happens for empty input, one element, missing target, or boundary values?
