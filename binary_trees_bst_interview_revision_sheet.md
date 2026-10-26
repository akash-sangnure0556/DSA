# Binary Trees & Binary Search Trees --- Interview Revision Sheet (C++)

**Scope:** interview-focused fundamentals plus famous LeetCode
Easy/Medium problems. The BST section is deliberately more detailed.
Solutions use LeetCode's usual `TreeNode` definition and assume standard
includes such as `<bits/stdc++.h>` and `using namespace std;`.

------------------------------------------------------------------------

# Part A --- Binary Tree Fundamentals

## 1. What is a binary tree?

A **binary tree** is a tree in which each node has at most two children:
`left` and `right`. A binary tree does **not** automatically have sorted
values.

``` cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

### Essential vocabulary

-   **Root:** top node.
-   **Parent / child:** directly connected node above / below another
    node.
-   **Leaf:** node with no children.
-   **Internal node:** node with at least one child.
-   **Depth:** number of edges from the root to a node (root depth is
    `0`).
-   **Height of a node:** longest downward path in edges from that node
    to a leaf. By this convention, a leaf's height is `0`; some
    implementations count nodes instead.
-   **Subtree:** a node and all descendants below it.
-   **Balanced tree:** for the common height-balanced definition, each
    node's left/right subtree heights differ by at most one.
-   **Complete tree:** every level is full except possibly the last,
    which is filled left to right.
-   **Full/proper tree:** each node has either zero or two children.
-   **Perfect tree:** every internal node has two children and all
    leaves are at the same depth.
-   **BST:** a binary tree with an ordering rule; see Part B.

### Useful properties

-   A tree with `n` nodes has `n - 1` edges (for `n > 0`).
-   A binary tree with height `h` has at most `2^(h+1) - 1` nodes when
    height is counted in edges.
-   Traversals usually take `O(n)` time.
-   Recursive traversal uses `O(h)` call-stack space, where `h` is tree
    height. A skewed tree can have `h = O(n)`.

## 2. How to think about recursion

For every recursive tree problem, ask: 1. **What does the function
return?** (height, sum, node pointer, boolean, list, etc.) 2. **What is
the base case?** Usually `if (root == nullptr)`. 3. **What answers come
from the left and right subtrees?** 4. **How do I combine those answers
for the current node?**

Common base cases:

``` cpp
if (root == nullptr) return 0;       // height in nodes, with empty height 0
if (root == nullptr) return true;     // empty subtree satisfies many properties
if (root == nullptr) return nullptr; // pointer-returning search
```

The correct return value depends on the function's meaning; do not copy
`0` blindly.

## 3. Four major traversals

For this example:

           1
          / \
         2   3
        / \
       4   5

-   **Preorder (Root, Left, Right):** `1 2 4 5 3`
-   **Inorder (Left, Root, Right):** `4 2 5 1 3`
-   **Postorder (Left, Right, Root):** `4 5 2 3 1`
-   **Level order (BFS):** `1 2 3 4 5`

### Recursive traversal templates

``` cpp
void preorder(TreeNode* root, vector<int>& ans) {
    if (!root) return;
    ans.push_back(root->val);
    preorder(root->left, ans);
    preorder(root->right, ans);
}

void inorder(TreeNode* root, vector<int>& ans) {
    if (!root) return;
    inorder(root->left, ans);
    ans.push_back(root->val);
    inorder(root->right, ans);
}

void postorder(TreeNode* root, vector<int>& ans) {
    if (!root) return;
    postorder(root->left, ans);
    postorder(root->right, ans);
    ans.push_back(root->val);
}
```

**Memory trick:** preorder processes before children; inorder between
children; postorder after children.

### Level-order traversal (BFS)

``` cpp
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> ans;
    if (!root) return ans;
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int sz = q.size();
        vector<int> level;
        while (sz--) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        ans.push_back(level);
    }
    return ans;
}
```

**Pattern:** capture `q.size()` at the start of each level, then process
exactly that many nodes.

## 4. Iterative traversals worth knowing

### Iterative preorder

``` cpp
vector<int> preorderIterative(TreeNode* root) {
    vector<int> ans;
    if (!root) return ans;
    stack<TreeNode*> st;
    st.push(root);
    while (!st.empty()) {
        TreeNode* node = st.top(); st.pop();
        ans.push_back(node->val);
        if (node->right) st.push(node->right);
        if (node->left) st.push(node->left); // left is processed first
    }
    return ans;
}
```

### Iterative inorder

``` cpp
vector<int> inorderIterative(TreeNode* root) {
    vector<int> ans;
    stack<TreeNode*> st;
    TreeNode* cur = root;
    while (cur || !st.empty()) {
        while (cur) {
            st.push(cur);
            cur = cur->left;
        }
        cur = st.top(); st.pop();
        ans.push_back(cur->val);
        cur = cur->right;
    }
    return ans;
}
```

### Iterative postorder (two stacks)

``` cpp
vector<int> postorderIterative(TreeNode* root) {
    vector<int> ans;
    if (!root) return ans;
    stack<TreeNode*> s1, s2;
    s1.push(root);
    while (!s1.empty()) {
        TreeNode* node = s1.top(); s1.pop();
        s2.push(node);
        if (node->left) s1.push(node->left);
        if (node->right) s1.push(node->right);
    }
    while (!s2.empty()) {
        ans.push_back(s2.top()->val);
        s2.pop();
    }
    return ans;
}
```

## 5. Binary-tree complexity cheat sheet

  -----------------------------------------------------------------------
  Operation                                Time               Extra space
  ------------------- ------------------------- -------------------------
  Any full traversal                     `O(n)`   `O(h)` recursion/stack;
                                                         BFS queue can be
                                                               `O(width)`

  Search in an                           `O(n)`          `O(h)` recursive
  arbitrary binary                              
  tree                                          

  Calculate height /                     `O(n)`          `O(h)` recursive
  count / sum                                   

  Level order                            `O(n)`                `O(width)`

  Skewed tree height                     `O(n)`                       ---

  Balanced tree                      `O(log n)`                       ---
  height                                        
  -----------------------------------------------------------------------

------------------------------------------------------------------------

# Part B --- Famous Binary Tree Easy/Medium Problems

## 1. LeetCode 104 --- Maximum Depth of Binary Tree (Easy)

``` cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        return 1 + max(maxDepth(root->left), maxDepth(root->right));
    }
};
```

**4-step approach** 1. An empty tree has depth `0`. 2. Recursively
calculate the left subtree depth. 3. Recursively calculate the right
subtree depth. 4. Return `1 + max(left, right)`. Time `O(n)`, recursion
space `O(h)`.

## 2. LeetCode 100 --- Same Tree (Easy)

``` cpp
class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (!p && !q) return true;
        if (!p || !q) return false;
        if (p->val != q->val) return false;
        return isSameTree(p->left, q->left) &&
               isSameTree(p->right, q->right);
    }
};
```

**4-step approach** 1. If both pointers are null, this pair matches. 2.
If exactly one is null, shapes differ. 3. If values differ, trees are
not the same. 4. Both left subtrees and both right subtrees must match.
Time `O(n)`.

## 3. LeetCode 226 --- Invert Binary Tree (Easy)

``` cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        swap(root->left, root->right);
        invertTree(root->left);
        invertTree(root->right);
        return root;
    }
};
```

**4-step approach** 1. Return null for an empty subtree. 2. Swap the
current node's left and right children. 3. Recursively invert both
resulting subtrees. 4. Return the original root pointer. Time `O(n)`,
space `O(h)`.

## 4. LeetCode 110 --- Balanced Binary Tree (Easy)

``` cpp
class Solution {
    int height(TreeNode* node) {
        if (!node) return 0;
        int l = height(node->left);
        if (l == -1) return -1;
        int r = height(node->right);
        if (r == -1 || abs(l - r) > 1) return -1;
        return 1 + max(l, r);
    }
public:
    bool isBalanced(TreeNode* root) {
        return height(root) != -1;
    }
};
```

**4-step approach** 1. Return height `0` for null; use `-1` as a
sentinel for unbalanced. 2. Get left height; immediately propagate `-1`
if left is unbalanced. 3. Get right height and fail if it is unbalanced
or the height difference exceeds one. 4. Return `1 + max(left,right)`
otherwise. Time `O(n)`; avoids recalculating heights.

## 5. LeetCode 543 --- Diameter of Binary Tree (Easy)

Diameter is the number of **edges** on the longest path between any two
nodes.

``` cpp
class Solution {
    int best = 0;
    int height(TreeNode* node) {
        if (!node) return 0;
        int l = height(node->left), r = height(node->right);
        best = max(best, l + r);
        return 1 + max(l, r);
    }
public:
    int diameterOfBinaryTree(TreeNode* root) {
        best = 0;
        height(root);
        return best;
    }
};
```

**4-step approach** 1. Recursively compute left and right subtree
heights. 2. A longest path passing through this node has
`leftHeight + rightHeight` edges. 3. Update a global maximum at every
node. 4. Return the maximum. Time `O(n)`, space `O(h)`.

## 6. LeetCode 102 --- Binary Tree Level Order Traversal (Medium)

``` cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> ans;
        if (!root) return ans;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int sz = q.size();
            vector<int> level;
            while (sz--) {
                TreeNode* cur = q.front(); q.pop();
                level.push_back(cur->val);
                if (cur->left) q.push(cur->left);
                if (cur->right) q.push(cur->right);
            }
            ans.push_back(level);
        }
        return ans;
    }
};
```

**4-step approach** 1. Push the root into a queue. 2. At each iteration,
save the queue size to know how many nodes belong to this level. 3. Pop
exactly that many nodes; record values and enqueue their children. 4.
Append the level vector. Time `O(n)`, space `O(width)`.

## 7. LeetCode 199 --- Binary Tree Right Side View (Medium)

``` cpp
class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> ans;
        if (!root) return ans;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; ++i) {
                TreeNode* cur = q.front(); q.pop();
                if (i == sz - 1) ans.push_back(cur->val);
                if (cur->left) q.push(cur->left);
                if (cur->right) q.push(cur->right);
            }
        }
        return ans;
    }
};
```

**4-step approach** 1. Process the tree level by level using BFS. 2.
Capture the number of nodes at the start of each level. 3. Add the value
of the last node processed at that level. 4. Return one visible value
per level. Time `O(n)`.

## 8. LeetCode 101 --- Symmetric Tree (Easy)

``` cpp
class Solution {
    bool mirror(TreeNode* a, TreeNode* b) {
        if (!a && !b) return true;
        if (!a || !b || a->val != b->val) return false;
        return mirror(a->left, b->right) &&
               mirror(a->right, b->left);
    }
public:
    bool isSymmetric(TreeNode* root) {
        return !root || mirror(root->left, root->right);
    }
};
```

**4-step approach** 1. Compare the left and right subtrees as mirrors.
2. Two null nodes match; one null node or different values fail. 3.
Compare `left->left` with `right->right`. 4. Compare `left->right` with
`right->left`. Time `O(n)`.

## 9. LeetCode 112 --- Path Sum (Easy)

``` cpp
class Solution {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (!root) return false;
        if (!root->left && !root->right)
            return targetSum == root->val;
        int remaining = targetSum - root->val;
        return hasPathSum(root->left, remaining) ||
               hasPathSum(root->right, remaining);
    }
};
```

**4-step approach** 1. An empty tree has no qualifying root-to-leaf
path. 2. At a leaf, check whether its value equals the remaining sum. 3.
Otherwise subtract the current node's value from the target. 4. Search
either child. Time `O(n)`, recursion space `O(h)`.

## 10. LeetCode 236 --- Lowest Common Ancestor of a Binary Tree (Medium)

``` cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root || root == p || root == q) return root;
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        if (left && right) return root;
        return left ? left : right;
    }
};
```

**4-step approach** 1. If root is null or equals either target, return
root. 2. Search both subtrees recursively. 3. If left and right both
return non-null, the current root is the LCA. 4. Otherwise propagate
whichever side returned a node. Time `O(n)`. Assumes both target nodes
exist in the tree.

## 11. LeetCode 105 --- Construct Binary Tree from Preorder and Inorder (Medium)

``` cpp
class Solution {
    unordered_map<int, int> pos;
    int pre = 0;

    TreeNode* build(vector<int>& preorder, int lo, int hi) {
        if (lo > hi) return nullptr;
        int value = preorder[preorderIndex++];
        TreeNode* root = new TreeNode(value);
        int mid = pos[value];
        root->left = build(preorder, lo, mid - 1);
        root->right = build(preorder, mid + 1, hi);
        return root;
    }
    int preorderIndex = 0;
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        pos.clear();
        preorderIndex = 0;
        for (int i = 0; i < (int)inorder.size(); ++i) pos[inorder[i]] = i;
        return build(preorder, 0, (int)inorder.size() - 1);
    }
};
```

**4-step approach** 1. Preorder's next value is the root of the current
subtree. 2. Use a hash map to find that value's position in inorder. 3.
Inorder values left of the root belong to the left subtree; values right
belong to the right. 4. Recursively build both ranges. `O(n)` time/space
with unique node values.

## 12. LeetCode 124 --- Binary Tree Maximum Path Sum (Hard --- optional, skip if focusing Easy/Medium)

This is a famous problem but **Hard**, so it is included only as an
optional later challenge.

``` cpp
class Solution {
    int best = INT_MIN;
    int gain(TreeNode* node) {
        if (!node) return 0;
        int left = max(0, gain(node->left));
        int right = max(0, gain(node->right));
        best = max(best, node->val + left + right);
        return node->val + max(left, right);
    }
public:
    int maxPathSum(TreeNode* root) {
        best = INT_MIN;
        gain(root);
        return best;
    }
};
```

**4-step approach** 1. Recursively get each child's maximum downward
gain, discarding negative gains. 2. A path passing through this node is
`node value + left gain + right gain`. 3. Update the global best using
that path. 4. Return only one branch's gain to the parent. Time `O(n)`.

------------------------------------------------------------------------

# Part C --- Binary Search Tree (BST): Detailed Concepts

## 1. What makes a tree a BST?

For the **strict BST convention** used in these notes: - Every value in
the left subtree is smaller than the current node. - Every value in the
right subtree is greater than the current node. - Both subtrees must
themselves be BSTs. - Duplicate-value policy varies by problem; unless
specified, assume distinct values.

Example:

            8
           / \
          3   10
         / \    \
        1   6    14
           / \   /
          4   7 13

### Why the BST property is global

For root `8`, `6` is allowed on the left and `10` on the right. A node
cannot be validated by comparing only with its parent: `4` must be less
than `8` as well as greater than `3`. Use lower/upper bounds to validate
the whole subtree.

### The key advantage: inorder is sorted

Inorder traversal of a BST yields values in ascending order under the
strict BST rule. This is useful for: - validating a BST; - finding the
kth smallest element; - finding predecessor/successor; - recovering
sorted order without sorting separately.

## 2. BST search

``` cpp
TreeNode* searchBST(TreeNode* root, int val) {
    while (root && root->val != val) {
        if (val < root->val) root = root->left;
        else root = root->right;
    }
    return root;
}
```

**4-step approach** 1. Start at the root. 2. If the target matches,
return this node. 3. If target is smaller, move left; if larger, move
right. 4. If null is reached, the value is absent. Time `O(h)`; balanced
`O(log n)`, skewed `O(n)`.

## 3. BST insertion

``` cpp
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    if (val < root->val) root->left = insertIntoBST(root->left, val);
    else if (val > root->val) root->right = insertIntoBST(root->right, val);
    return root; // duplicate ignored under this convention
}
```

**4-step approach** 1. If the correct child position is null, create the
new node there. 2. Compare the inserted value with the current node. 3.
Recurse left for smaller values and right for larger values. 4. Reattach
the returned child pointer and return root. Time `O(h)`.

## 4. BST deletion --- the three cases

Deletion is a particularly important interview concept: 1. **Leaf:**
delete the node; replace it with null. 2. **One child:** replace the
node with its only child. 3. **Two children:** replace its value with
the inorder successor (minimum in the right subtree) or predecessor
(maximum in the left), then delete that replacement node from its
original position.

``` cpp
class Solution {
    TreeNode* minNode(TreeNode* node) {
        while (node && node->left) node = node->left;
        return node;
    }
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (!root) return nullptr;

        if (key < root->val) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->val) {
            root->right = deleteNode(root->right, key);
        } else {
            if (!root->left) {
                TreeNode* right = root->right;
                delete root;
                return right;
            }
            if (!root->right) {
                TreeNode* left = root->left;
                delete root;
                return left;
            }
            TreeNode* succ = minNode(root->right);
            root->val = succ->val;
            root->right = deleteNode(root->right, succ->val);
        }
        return root;
    }
};
```

**4-step approach** 1. Search for the key using BST ordering. 2. If the
node has zero or one child, replace it with the available child (or
null). 3. If it has two children, copy the minimum value from the right
subtree. 4. Delete that successor from the right subtree and return the
updated root. Time `O(h)`.

**C++ note:** `delete root` is valid for nodes allocated with `new`, as
in this example. Some interview platform conventions don't require
explicit freeing; follow the expected ownership model.

## 5. Validate BST --- LeetCode 98 (Medium)

``` cpp
class Solution {
    bool valid(TreeNode* node, long long low, long long high) {
        if (!node) return true;
        if (node->val <= low || node->val >= high) return false;
        return valid(node->left, low, node->val) &&
               valid(node->right, node->val, high);
    }
public:
    bool isValidBST(TreeNode* root) {
        return valid(root, LLONG_MIN, LLONG_MAX);
    }
};
```

**4-step approach** 1. Give each node an allowed exclusive range
`(low, high)`. 2. Reject a value that is outside or on a boundary of
that range. 3. The left subtree gets `(low, node->val)`; the right gets
`(node->val, high)`. 4. Both subtrees must pass. Time `O(n)`, recursion
space `O(h)`.

**Common bug:** checking only `root->left->val < root->val` and
`root->right->val > root->val` is insufficient because deeper
descendants must obey ancestor bounds too.

## 6. Minimum and maximum in a BST

``` cpp
TreeNode* findMin(TreeNode* root) {
    if (!root) return nullptr;
    while (root->left) root = root->left;
    return root;
}
TreeNode* findMax(TreeNode* root) {
    if (!root) return nullptr;
    while (root->right) root = root->right;
    return root;
}
```

**4-step approach** 1. The minimum is the leftmost node. 2. The maximum
is the rightmost node. 3. Follow left pointers for minimum or right
pointers for maximum. 4. Stop when the next pointer is null. Time
`O(h)`.

## 7. Floor and ceil in a BST

-   **Floor(x):** greatest value `<= x`.
-   **Ceil(x):** smallest value `>= x`.
-   If no qualifying value exists, return `-1` (a common problem
    convention; if `-1` can be a valid key, use an optional/pair
    instead).

### Ceil

``` cpp
int findCeil(TreeNode* root, int x) {
    int ans = -1;
    while (root) {
        if (root->val == x) return x;
        if (root->val > x) {
            ans = root->val;   // candidate
            root = root->left; // maybe a smaller valid ceil exists
        } else {
            root = root->right;
        }
    }
    return ans;
}
```

### Floor

``` cpp
int findFloor(TreeNode* root, int x) {
    int ans = -1;
    while (root) {
        if (root->val == x) return x;
        if (root->val < x) {
            ans = root->val;    // candidate
            root = root->right; // maybe a larger valid floor exists
        } else {
            root = root->left;
        }
    }
    return ans;
}
```

**4-step approach** 1. Keep an answer candidate initialized to the
problem's "not found" value. 2. For ceil, a node greater than `x` is a
candidate; go left to seek a smaller candidate. 3. For floor, a node
smaller than `x` is a candidate; go right to seek a larger candidate. 4.
When equal, return immediately; otherwise return the best candidate.
Time `O(h)`.

## 8. Inorder successor and predecessor

-   **Successor:** smallest key strictly greater than the given key.
-   **Predecessor:** largest key strictly smaller than the given key.

For a node pointer: - Successor, if a right child exists: minimum node
in its right subtree. - Predecessor, if a left child exists: maximum
node in its left subtree. - Otherwise, move upward to the nearest
ancestor for which the node lies in its left subtree (successor) or
right subtree (predecessor).

If only the root and target **value** are supplied, you can find the
successor candidate during a BST search:

``` cpp
TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
    TreeNode* ans = nullptr;
    while (root) {
        if (p->val < root->val) {
            ans = root;
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return ans;
}
```

This version assumes the target value is in the BST and distinct. Time
`O(h)`.

## 9. Lowest Common Ancestor in a BST --- LeetCode 235 (Medium)

``` cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        while (root) {
            if (p->val < root->val && q->val < root->val)
                root = root->left;
            else if (p->val > root->val && q->val > root->val)
                root = root->right;
            else
                return root;
        }
        return nullptr;
    }
};
```

**4-step approach** 1. If both target values are smaller than root, LCA
is in the left subtree. 2. If both are larger, LCA is in the right
subtree. 3. Otherwise, they split across sides or one equals root; root
is the LCA. 4. Return it. Time `O(h)`. Assumes both target nodes exist.

## 10. Kth smallest in a BST --- LeetCode 230 (Medium)

``` cpp
class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        stack<TreeNode*> st;
        TreeNode* cur = root;
        while (cur || !st.empty()) {
            while (cur) {
                st.push(cur);
                cur = cur->left;
            }
            cur = st.top(); st.pop();
            if (--k == 0) return cur->val;
            cur = cur->right;
        }
        return -1; // problem normally guarantees valid k
    }
};
```

**4-step approach** 1. Inorder traversal of a BST visits values in
sorted ascending order. 2. Use an iterative inorder stack to avoid
storing the whole traversal. 3. Decrement `k` each time a node is
visited. 4. Return the node value when `k` reaches zero. Time `O(h+k)`
in the usual bound; worst case `O(n)`, stack `O(h)`.

## 11. Range Sum of BST --- LeetCode 938 (Easy)

``` cpp
class Solution {
public:
    int rangeSumBST(TreeNode* root, int low, int high) {
        if (!root) return 0;
        if (root->val < low) return rangeSumBST(root->right, low, high);
        if (root->val > high) return rangeSumBST(root->left, low, high);
        return root->val
             + rangeSumBST(root->left, low, high)
             + rangeSumBST(root->right, low, high);
    }
};
```

**4-step approach** 1. Return zero for a null node. 2. If value is below
`low`, its left subtree is also too small; only search right. 3. If
value is above `high`, its right subtree is too large; only search left.
4. Otherwise add the node and search both sides. Average `O(n)`
worst-case `O(n)`; pruning can reduce visits.

## 12. Search in a BST --- LeetCode 700 (Easy)

``` cpp
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        while (root && root->val != val) {
            root = (val < root->val) ? root->left : root->right;
        }
        return root;
    }
};
```

**4-step approach** 1. Compare the target with the current node. 2.
Return current node if values match. 3. Move left if target is smaller,
right if larger. 4. Return null if not found. Time `O(h)`.

## 13. Insert into a BST --- LeetCode 701 (Medium)

``` cpp
class Solution {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (!root) return new TreeNode(val);
        if (val < root->val) root->left = insertIntoBST(root->left, val);
        else if (val > root->val) root->right = insertIntoBST(root->right, val);
        return root;
    }
};
```

**4-step approach** 1. Find the first null child in the direction
required by BST ordering. 2. Create the node at that position. 3.
Reconnect the recursive result to the corresponding child pointer. 4.
Return the original root. Time `O(h)`.

## 14. Delete Node in a BST --- LeetCode 450 (Medium)

Use the complete deletion template in **Part C, Section 4**. It covers
leaf, one-child, and two-child cases. **4-step approach** 1. Search by
BST ordering. 2. For zero/one child, return the surviving child. 3. For
two children, copy the inorder successor (minimum in right subtree). 4.
Delete the successor from its original location and return the root.
Time `O(h)`.

## 15. Convert Sorted Array to BST --- LeetCode 108 (Easy)

``` cpp
class Solution {
    TreeNode* build(vector<int>& nums, int l, int r) {
        if (l > r) return nullptr;
        int mid = l + (r - l) / 2;
        TreeNode* root = new TreeNode(nums[mid]);
        root->left = build(nums, l, mid - 1);
        root->right = build(nums, mid + 1, r);
        return root;
    }
public:
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        return build(nums, 0, (int)nums.size() - 1);
    }
};
```

**4-step approach** 1. Pick the middle element as the root to keep the
tree balanced. 2. Values before the middle form the left subtree. 3.
Values after the middle form the right subtree. 4. Recurse on each half.
Time `O(n)`, recursion space `O(log n)` for the balanced result.

## 16. Two Sum IV --- Input is a BST --- LeetCode 653 (Easy)

``` cpp
class Solution {
public:
    bool findTarget(TreeNode* root, int k) {
        unordered_set<int> seen;
        stack<TreeNode*> st;
        if (root) st.push(root);
        while (!st.empty()) {
            TreeNode* node = st.top(); st.pop();
            if (seen.count(k - node->val)) return true;
            seen.insert(node->val);
            if (node->left) st.push(node->left);
            if (node->right) st.push(node->right);
        }
        return false;
    }
};
```

**4-step approach** 1. Traverse every node using DFS/stack. 2. For value
`x`, check whether `k-x` has already been seen. 3. If yes, two distinct
visited nodes sum to `k`; otherwise store `x`. 4. Return false if no
pair is found. Time `O(n)`, space `O(n)`.

## 17. Minimum Absolute Difference in BST --- LeetCode 530 (Easy)

``` cpp
class Solution {
    TreeNode* prev = nullptr;
    int ans = INT_MAX;
    void inorder(TreeNode* node) {
        if (!node) return;
        inorder(node->left);
        if (prev) ans = min(ans, node->val - prev->val);
        prev = node;
        inorder(node->right);
    }
public:
    int getMinimumDifference(TreeNode* root) {
        prev = nullptr;
        ans = INT_MAX;
        inorder(root);
        return ans;
    }
};
```

**4-step approach** 1. Inorder traversal visits BST values in sorted
order. 2. Keep the previous visited node. 3. Update the answer using
`current value - previous value`. 4. Return the smallest adjacent
difference. Time `O(n)`, space `O(h)`.

## 18. Validate / recover BST: a common conceptual contrast

Validation checks whether ordering is correct; recovery (LeetCode 99) is
**Medium** and asks to fix two swapped values. For interview
fundamentals, prioritise validation first. If studying recovery, compare
the inorder sequence: swapped nodes create one or two inversions;
identify the misplaced nodes and swap their values.

------------------------------------------------------------------------

# Part D --- BST Concepts Interviewers Commonly Test

## 1. BST operation complexity

  -----------------------------------------------------------------------
  Operation                        Balanced BST     Worst-case skewed BST
  ------------------- ------------------------- -------------------------
  Search                             `O(log n)`                    `O(n)`

  Insert                             `O(log n)`                    `O(n)`

  Delete                             `O(log n)`                    `O(n)`

  Min/max                            `O(log n)`                    `O(n)`

  Floor/ceil                         `O(log n)`                    `O(n)`

  kth smallest using     `O(h+k)` typical early                    `O(n)`
  inorder                    stop; worst `O(n)` 
  -----------------------------------------------------------------------

A plain BST is **not guaranteed balanced**. AVL trees and Red-Black
trees maintain balance, but their rotations/invariants are beyond the
core LeetCode BST set.

## 2. Predecessor vs successor

-   **Predecessor:** immediately previous key in sorted order.
-   **Successor:** immediately next key in sorted order.
-   If a right subtree exists, successor is its leftmost node.
-   If a left subtree exists, predecessor is its rightmost node.
-   Otherwise, walk ancestors / search from root to find the nearest
    valid candidate.

## 3. Common BST patterns

-   **Search/insert/delete:** use comparison to eliminate one subtree at
    each step.
-   **Validation:** carry strict lower and upper bounds.
-   **Sorted output / kth smallest / min difference:** inorder
    traversal.
-   **Floor:** remember candidates smaller than target and move right.
-   **Ceil:** remember candidates larger than target and move left.
-   **Range queries:** prune subtrees that cannot contain valid values.
-   **LCA:** move left if both targets are smaller; right if both are
    larger; otherwise current node is split point.
-   **Balanced construction from sorted values:** choose the midpoint
    recursively.

## 4. BST pitfalls

1.  Inorder sortedness is a valid test for a strict BST, but be careful
    with duplicates and the problem's duplicate policy.
2.  A local parent-child comparison does not validate a whole BST.
3.  Use `long long` bounds for validation so `INT_MIN` and `INT_MAX` are
    valid node values.
4.  Do not confuse node count and edge count when discussing
    height/depth/diameter.
5.  For floor/ceil, preserve the best candidate before moving to a
    subtree that may improve it.
6.  For deletion with two children, copying successor value is not
    enough; the original successor must also be removed.
7.  `root = insertIntoBST(root, val)` is important because inserting
    into an empty tree changes the root.
8.  A normal BST can become skewed; do not promise `O(log n)` unless
    balance is guaranteed.
9.  In recursive code, return and reconnect child pointers:
    `root->left = recurse(root->left, ...)`.
10. Confirm whether the platform expects `TreeNode*`, a `Node*`, or a
    `Node` structure with a different constructor.

------------------------------------------------------------------------

# Part E --- Famous Easy/Medium Problem Checklist

## Binary Trees

  ------------------------------------------------------------------------
  LeetCode         Problem                     Difficulty Pattern
  ---------------- ---------------- --------------------- ----------------
  104              Maximum Depth of                  Easy Recursion /
                   Binary Tree                            height

  100              Same Tree                         Easy Paired recursion

  226              Invert Binary                     Easy Swap children
                   Tree                                   

  101              Symmetric Tree                    Easy Mirror recursion

  110              Balanced Binary                   Easy Height +
                   Tree                                   sentinel

  543              Diameter of                       Easy Height + global
                   Binary Tree                            answer

  112              Path Sum                          Easy Root-to-leaf
                                                          recursion

  102              Binary Tree                     Medium BFS by level
                   Level Order                            
                   Traversal                              

  199              Binary Tree                     Medium BFS last node
                   Right Side View                        per level

  236              Lowest Common                   Medium Postorder
                   Ancestor of                            recursion
                   Binary Tree                            

  105              Construct Tree                  Medium Divide and
                   from Preorder                          conquer
                   and Inorder                            

  144              Binary Tree                       Easy Traversal
                   Preorder                               
                   Traversal                              

  94               Binary Tree                       Easy Traversal
                   Inorder                                
                   Traversal                              

  145              Binary Tree                       Easy Traversal
                   Postorder                              
                   Traversal                              

  572              Subtree of                        Easy Same-tree
                   Another Tree                           comparison
  ------------------------------------------------------------------------

## Binary Search Trees --- prioritise these

  ------------------------------------------------------------------------
  LeetCode         Problem                     Difficulty Pattern
  ---------------- ---------------- --------------------- ----------------
  700              Search in a                       Easy Ordered search
                   Binary Search                          
                   Tree                                   

  938              Range Sum of BST                  Easy Pruning

  108              Convert Sorted                    Easy Divide and
                   Array to BST                           conquer

  653              Two Sum IV ---                    Easy DFS + hash set
                   Input is a BST                         

  530              Minimum Absolute                  Easy Inorder /
                   Difference in                          previous
                   BST                                    

  501              Find Mode in                      Easy Inorder
                   Binary Search                          frequencies
                   Tree                                   

  783              Minimum Distance                  Easy Inorder adjacent
                   Between BST                            gap
                   Nodes                                  

  235              Lowest Common                   Medium Ordered descent
                   Ancestor of a                          
                   BST                                    

  98               Validate Binary                 Medium Bounds
                   Search Tree                            

  230              Kth Smallest                    Medium Inorder +
                   Element in a BST                       counter

  701              Insert into a                   Medium Recursive
                   Binary Search                          insertion
                   Tree                                   

  450              Delete Node in a                Medium Three deletion
                   BST                                    cases

  173              Binary Search                   Medium Controlled
                   Tree Iterator                          inorder stack

  669              Trim a Binary                   Medium Pruning
                   Search Tree                            

  897              Increasing Order                  Easy Inorder
                   Search Tree                            relinking

  99               Recover Binary                  Medium Inorder
                   Search Tree                            inversions
  ------------------------------------------------------------------------

------------------------------------------------------------------------

# Part F --- Quick Mock-Interview Questions

Try answering these aloud before checking your notes: 1. Why does
inorder traversal of a BST produce sorted order? 2. Why is comparing
each node only with its parent insufficient to validate a BST? 3. What
are the three BST deletion cases? 4. How would you find the kth smallest
value without sorting all values? 5. When can a BST operation degrade
from `O(log n)` to `O(n)`? 6. What is the difference between a binary
tree and a BST? 7. What is the difference between depth and height? 8.
Why does BFS use a queue while iterative DFS typically uses a stack? 9.
How do you find the LCA in a general binary tree versus a BST? 10. How
can you build a height-balanced BST from a sorted array?

## Recommended revision order

1.  Traversals: preorder, inorder, postorder, level order.
2.  Easy binary-tree recursion: max depth, same tree, invert tree,
    symmetry.
3.  Height patterns: balanced tree and diameter.
4.  BFS patterns: level order and right-side view.
5.  General-tree LCA and path sum.
6.  BST search, insert, min/max, floor/ceil.
7.  BST validation, kth smallest, LCA, range sum.
8.  BST deletion and successor/predecessor.
9.  Finish with Two Sum IV, minimum difference, and sorted-array-to-BST.

**Interview rule:** Explain the invariant first, then the base case,
then how the left/right results combine, then complexity. For BST
questions, explicitly state how ordering lets you skip one subtree.
