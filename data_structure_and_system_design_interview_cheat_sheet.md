# Data Structure Design + System Design Interview Cheat Sheet
## C++ solutions for 8 coding problems + simple blueprints for 10 system-design questions

Built for interview revision: first understand the idea, then memorize the operations and complexity. Try to explain each solution aloud before coding.

---

# Part A — Eight coding problems

## 1. LRU Cache — LeetCode 146 (Medium)

### What is the question?
Build a cache with a fixed capacity. `get(key)` returns the value if present, otherwise `-1`. `put(key, value)` inserts or updates an item. When full, remove the **least recently used** item.

### Easy idea
We need both:
- Fast lookup by key → `unordered_map`.
- Fast movement/removal by usage order → doubly linked list.

Keep the **most recently used** item at the front and the **least recently used** item at the back. The map stores each key's list iterator.

### C++ solution
```cpp
```cpp
#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

class LRUCache {
private:
    int capacity;

    // Stores {key, value}
    // Front = Most Recently Used
    // Back  = Least Recently Used
    list<pair<int, int>> cache;

    // Maps each key to its position in the linked list
    unordered_map<int, list<pair<int, int>>::iterator> mp;

public:
    // Constructor
    LRUCache(int cap) {
        capacity = cap;
    }

    // Move an existing item to the front
    void makeRecent(list<pair<int, int>>::iterator it) {
        cache.splice(cache.begin(), cache, it);
    }

    // Get the value for a key
    int get(int key) {
        auto it = mp.find(key);

        // Key does not exist
        if (it == mp.end()) {
            return -1;
        }

        // Key exists, so make it most recently used
        makeRecent(it->second);

        // Return the value
        return it->second->second;
    }

    // Insert a new key or update an existing key
    void put(int key, int value) {
        auto it = mp.find(key);

        // If the key already exists
        if (it != mp.end()) {
            it->second->second = value;
            makeRecent(it->second);
            return;
        }

        // If cache is full, remove the least recently used item
        if (cache.size() == capacity) {
            int oldKey = cache.back().first;

            mp.erase(oldKey);
            cache.pop_back();
        }

        // Insert the new item at the front
        cache.push_front({key, value});

        // Store the iterator of the new item
        mp[key] = cache.begin();
    }

    // Display the cache from most to least recently used
    void display() {
        cout << "Cache: ";

        for (auto item : cache) {
            cout << "[" << item.first << ":" << item.second << "] ";
        }

        cout << endl;
    }
};

int main() {
    LRUCache cache(2);  // Cache capacity = 2

    cache.put(1, 100);
    cache.display();

    cache.put(2, 200);
    cache.display();

    cout << "Get key 1: " << cache.get(1) << endl;
    cache.display();

    cache.put(3, 300);
    cache.display();

    cout << "Get key 2: " << cache.get(2) << endl;

    cache.put(4, 400);
    cache.display();

    cout << "Get key 1: " << cache.get(1) << endl;
    cout << "Get key 3: " << cache.get(3) << endl;
    cout << "Get key 4: " << cache.get(4) << endl;

    return 0;
}
```
```

### Four steps to explain
1. Use a hash map for O(1) average key lookup and a doubly linked list for recency order.
2. Put the newest/recently used item at the front; the oldest item stays at the back.
3. On `get`, return the value and move that node to the front.
4. On `put`, update/move an existing node, or evict the back node if full and insert the new node at the front.

### Complexity
- `get`: O(1) average.
- `put`: O(1) average.
- Space: O(capacity).

### Edge cases
- Capacity is positive (usual problem constraint).
- Missing key returns `-1`.
- Updating an existing key must not create a duplicate.
- A successful `get` changes recency.

---

## 2. Insert Delete GetRandom O(1) — LeetCode 380 (Medium)

### What is the question?
Implement `insert`, `remove`, and `getRandom`. Each should run in O(1) average time. Values are unique.

### Easy idea
Use a vector for random indexing and a hash map from value to its vector index. To remove from the middle, swap it with the last value and `pop_back()`.

### C++ solution
```cpp
class RandomizedSet {
    vector<int> values;
    unordered_map<int, int> indexOf;
    mt19937 rng{random_device{}()};

public:
    bool insert(int val) {
        if (indexOf.count(val)) return false;
        indexOf[val] = values.size();
        values.push_back(val);
        return true;
    }

    bool remove(int val) {
        auto it = indexOf.find(val);
        if (it == indexOf.end()) return false;

        int idx = it->second;
        int lastVal = values.back();

        values[idx] = lastVal;
        indexOf[lastVal] = idx;

        values.pop_back();
        indexOf.erase(it);
        return true;
    }

    int getRandom() {
        uniform_int_distribution<int> dist(0, (int)values.size() - 1);
        return values[dist(rng)]; // assumes set is non-empty
    }
};
```

### Four steps to explain
1. Store values in a vector so any index can be selected randomly in O(1).
2. Store each value's current index in a hash map for fast lookup.
3. To remove a value, overwrite its slot with the last vector value, update that value's map index, then pop the last slot.
4. `getRandom` chooses a uniformly random vector index.

### Complexity
- `insert`, `remove`, `getRandom`: O(1) average.
- Space: O(n).

### Edge cases
- Duplicate insertion returns `false`.
- Removing an absent value returns `false`.
- `getRandom` assumes at least one element, as guaranteed by the usual problem constraints.

---

## 3. Min Stack — LeetCode 155 (Medium)

### What is the question?
Implement `push`, `pop`, `top`, and `getMin`, all in O(1).

### Easy idea
Keep a second stack containing the minimum value at each level. The top of the second stack is the current minimum.

### C++ solution
```cpp
class MinStack {
    stack<int> values;
    stack<int> mins;

public:
    void push(int val) {
        values.push(val);
        if (mins.empty() || val <= mins.top()) {
            mins.push(val);
        }
    }

    void pop() {
        if (values.top() == mins.top()) mins.pop();
        values.pop();
    }

    int top() {
        return values.top();
    }

    int getMin() {
        return mins.top();
    }
};
```

### Four steps to explain
1. Use one stack for all values and another stack to track minimums.
2. Push a value onto the minimum stack if it is less than or equal to the current minimum.
3. When popping, if the removed value equals the minimum stack's top, pop that too.
4. The minimum is always available at `mins.top()`.

### Complexity
- Every operation: O(1).
- Extra space: O(n).

### Edge cases
- Operations are normally called only when the stack is non-empty.
- Use `<=`, not just `<`, so duplicate minimum values are tracked correctly.

---

## 4. Time Based Key-Value Store — LeetCode 981 (Medium)

### What is the question?
`set(key, value, timestamp)` stores a value. `get(key, timestamp)` returns the value associated with the largest stored timestamp that is **less than or equal to** the requested timestamp. Return `""` if none exists.

### Easy idea
For each key, store `(timestamp, value)` pairs in increasing timestamp order. Binary-search for the last timestamp `<=` the query timestamp.

### C++ solution
```cpp
class TimeMap {
    unordered_map<string, vector<pair<int, string>>> data;

public:
    void set(string key, string value, int timestamp) {
        data[key].push_back({timestamp, value});
    }

    string get(string key, int timestamp) {
        auto it = data.find(key);
        if (it == data.end()) return "";

        const auto& arr = it->second;
        int left = 0, right = (int)arr.size() - 1;
        int answer = -1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (arr[mid].first <= timestamp) {
                answer = mid;
                left = mid + 1; // search for a later valid timestamp
            } else {
                right = mid - 1;
            }
        }

        return answer == -1 ? "" : arr[answer].second;
    }
};
```

### Four steps to explain
1. Use a hash map to find the history for a key.
2. Store each key's timestamp/value pairs in timestamp order; the problem guarantees `set` timestamps increase.
3. Binary-search for the rightmost timestamp that is `<=` the requested timestamp.
4. Return its value, or an empty string if no timestamp qualifies.

### Complexity
- `set`: O(1) amortized.
- `get`: O(log m), where m is the number of entries for that key.
- Space: O(total stored entries).

### Edge cases
- Key has never been stored.
- Query timestamp is before the first timestamp.
- Query timestamp exactly matches a stored timestamp.

---

## 5. Design Circular Queue — LeetCode 622 (Medium)

### What is the question?
Implement a fixed-capacity queue supporting `enQueue`, `deQueue`, `Front`, `Rear`, `isEmpty`, and `isFull`.

### Easy idea
Use a fixed-size vector and three values: `head` (front index), `count` (number of elements), and capacity `k`. The next position wraps around using modulo.

### C++ solution
```cpp
class MyCircularQueue {
    vector<int> data;
    int head = 0;
    int count = 0;
    int capacity;

public:
    MyCircularQueue(int k) : data(k), capacity(k) {}

    bool enQueue(int value) {
        if (isFull()) return false;
        int tail = (head + count) % capacity;
        data[tail] = value;
        ++count;
        return true;
    }

    bool deQueue() {
        if (isEmpty()) return false;
        head = (head + 1) % capacity;
        --count;
        return true;
    }

    int Front() {
        return isEmpty() ? -1 : data[head];
    }

    int Rear() {
        if (isEmpty()) return -1;
        int tail = (head + count - 1) % capacity;
        return data[tail];
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == capacity;
    }
};
```

### Four steps to explain
1. Allocate a fixed-size vector and track the front index and element count.
2. Insert at `(head + count) % capacity`, then increment count.
3. Remove by advancing `head = (head + 1) % capacity`, then decrement count.
4. Use count to distinguish empty from full; modulo wraps indices around.

### Complexity
- Every operation: O(1).
- Space: O(k).

### Edge cases
- Queue is full or empty.
- Front/rear on an empty queue return `-1`.
- The standard problem gives `k >= 1`, avoiding modulo by zero.

---

## 6. Copy List with Random Pointer — LeetCode 138 (Medium)

### What is the question?
Each node has `next` and `random` pointers. Create a deep copy where every new node is distinct from the original, and all `next` and `random` links point to the corresponding copied nodes.

### Easy idea
Use a hash map from each original node pointer to its copied node pointer. First create all copies, then wire up their `next` and `random` pointers.

### C++ solution
```cpp
class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;

        unordered_map<Node*, Node*> copied;

        // Pass 1: create a copy for every original node.
        for (Node* cur = head; cur != nullptr; cur = cur->next) {
            copied[cur] = new Node(cur->val);
        }

        // Pass 2: connect the copied nodes.
        for (Node* cur = head; cur != nullptr; cur = cur->next) {
            copied[cur]->next =
                (cur->next == nullptr) ? nullptr : copied[cur->next];

            copied[cur]->random =
                (cur->random == nullptr) ? nullptr : copied[cur->random];
        }

        return copied[head];
    }
};
```

### Four steps to explain
1. Create a hash map mapping every original node pointer to its newly allocated copy.
2. First pass creates all copied nodes, without connecting them.
3. Second pass assigns each copied node's `next` and `random` through the map.
4. Return the copied head; the original list remains unchanged.

### Complexity
- Time: O(n).
- Extra space: O(n).

### Edge cases
- Empty list returns `nullptr`.
- A node may point randomly to itself.
- Multiple nodes may point to the same random target.

**Note:** LeetCode supplies the `Node` definition. The expected fields are `val`, `next`, and `random`.

---

## 7. Top K Frequent Elements — LeetCode 347 (Medium)

### What is the question?
Return the `k` most frequent values in an integer array. The answer may be in any order.

### Easy idea
Count each value with a hash map. Keep a min-heap of at most `k` pairs `(frequency, value)`. If the heap gets too large, remove the least frequent entry.

### C++ solution
```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        for (int x : nums) ++freq[x];

        // Min-heap: smallest frequency is at the top.
        using P = pair<int, int>; // {frequency, value}
        priority_queue<P, vector<P>, greater<P>> pq;

        for (auto& [value, count] : freq) {
            pq.push({count, value});
            if ((int)pq.size() > k) pq.pop();
        }

        vector<int> answer;
        while (!pq.empty()) {
            answer.push_back(pq.top().second);
            pq.pop();
        }

        return answer;
    }
};
```

### Four steps to explain
1. Count occurrences of every number using a hash map.
2. Insert each `(frequency, value)` into a min-heap.
3. Keep the heap size at most `k`; when it exceeds `k`, remove the smallest frequency.
4. The heap now contains the `k` most frequent values; pop them into the answer.

### Complexity
Let `n` be the array length and `u` the number of unique values.
- Time: O(n + u log k).
- Space: O(u + k).

### Edge cases
- `k` is valid and no larger than the number of unique values under the usual constraints.
- The answer order does not matter.
- For equal frequencies, any valid selection consistent with the problem's constraints is acceptable.

---

## 8. Find Median from Data Stream — LeetCode 295 (Hard)

### What is the question?
Numbers arrive one at a time. Support `addNum(num)` and `findMedian()` efficiently.

### Easy idea
Use two heaps:
- `small`: max-heap containing the smaller half.
- `large`: min-heap containing the larger half.

Keep `small` the same size as `large`, or one element larger. The median is then available at the heap tops.

### C++ solution
```cpp
class MedianFinder {
    priority_queue<int> small; // max-heap: lower half
    priority_queue<int, vector<int>, greater<int>> large; // min-heap: upper half

public:
    void addNum(int num) {
        if (small.empty() || num <= small.top()) {
            small.push(num);
        } else {
            large.push(num);
        }

        // Rebalance so small has either equal size or one extra.
        if (small.size() > large.size() + 1) {
            large.push(small.top());
            small.pop();
        } else if (large.size() > small.size()) {
            small.push(large.top());
            large.pop();
        }
    }

    double findMedian() {
        if (small.size() > large.size()) {
            return small.top();
        }

        return ((long long)small.top() + large.top()) / 2.0;
    }
};
```

### Four steps to explain
1. Split values into a lower half in a max-heap and an upper half in a min-heap.
2. Insert into the lower heap if the number is no greater than its top; otherwise insert into the upper heap.
3. Rebalance so the lower heap has either the same number of elements or exactly one more.
4. If counts differ, the lower heap's top is the median; otherwise average both heap tops.

### Complexity
- `addNum`: O(log n).
- `findMedian`: O(1).
- Space: O(n).

### Edge cases
- The usual problem calls `findMedian` only after at least one number has been added.
- Cast to `long long` before adding heap tops to avoid integer overflow.
- Duplicates are allowed.

---

# Part B — System design interview cheat sheet

For each design question, answer in this order:
1. Clarify requirements and expected scale.
2. Define APIs and data model.
3. Draw the high-level components and request/data flow.
4. Explain failure cases, concurrency, and trade-offs.
5. Mention monitoring and security where relevant.

For a 30–45 minute interview, start with a simple correct design and add scale only when needed.

---

## 1. Design an LRU Cache

### Requirements
- `get(key)` returns the value or a miss.
- `put(key, value)` inserts/updates.
- Evict the least recently used item at capacity.
- Target O(1) average `get` and `put`.

### Components and data structures
- Hash map: `key -> list node`.
- Doubly linked list: MRU at front, LRU at back.
- Capacity limit.

### Request flow
- `get`: map lookup → miss returns absent; hit moves node to front → return value.
- `put`: existing key updates and moves front; new key evicts back if full → inserts front and map entry.

### Complexity
- `get` and `put`: O(1) average; memory O(capacity).

### Discuss if asked
- What happens with capacity zero?
- Is it thread-safe? Add a mutex for a simple in-process implementation.
- A distributed cache adds networking, partitioning, replication, expiry, and consistency concerns; it is a different problem.

---

## 2. Design an LFU Cache

### Requirements
Evict the item with the lowest access frequency. If multiple items share the lowest frequency, evict the least recently used among them.

### Simple efficient design
Maintain:
- `key -> {value, frequency, position}`.
- `frequency -> doubly linked list of keys`.
- `minFrequency`, the lowest frequency currently present.

### Operations
- On `get` or update of an existing key: remove it from its current frequency list, increment frequency, and add it to the front of the next frequency list.
- On insertion when full: evict the least-recently-used key from the list for `minFrequency`.
- A newly inserted key starts at frequency 1, so set `minFrequency = 1`.

### Complexity target
O(1) average for `get` and `put`, with hash maps and linked lists.

### Important distinction
LRU evicts by **recency**. LFU evicts by **frequency**, using recency to break ties. Discuss how counters might age or reset in a long-running production cache.

---

## 3. Design a Rate Limiter

### Requirements
Limit how often a user/client can call an API. Example: 100 requests per minute per user.

### Simple design
- API gateway/middleware extracts client or user ID.
- Rate-limiter service checks a counter or token bucket.
- Store state in a fast shared store such as Redis when multiple application servers must share limits.
- If allowed, forward request; otherwise return HTTP `429 Too Many Requests`, optionally with retry information.

### Choose an algorithm
- **Fixed window:** simple counter per time bucket; can allow bursts at bucket boundaries.
- **Sliding-window log:** records request timestamps; accurate but stores more data.
- **Sliding-window counter:** estimates a rolling window with less memory.
- **Token bucket:** tokens refill at a configured rate; supports controlled bursts.

### Key design decisions
- Key by user, API key, IP, or combination.
- Use atomic updates/operations to avoid concurrent requests bypassing the limit.
- Define behavior if the shared store is unavailable: fail open or fail closed based on risk.
- Add TTLs so old counters expire; return `429` when over limit.

---

## 4. Design a Logging System

### Requirements
Accept logs from many services, process high volume, search/filter logs, and retain them for a defined period.

### High-level architecture
Applications → logging agent/collector → durable message queue → processing/indexing pipeline → log storage/search index → query UI/API.

### Data model
Typical fields: timestamp, service, host, severity, trace/request ID, message, structured attributes.

### Important decisions
- Buffer and batch writes to reduce overhead.
- Use a durable queue to absorb traffic spikes and decouple producers from storage.
- Partition by time and possibly service/tenant.
- Apply retention and lifecycle policies; archive older data cheaply.
- Redact secrets and sensitive data; enforce access control.
- Monitor dropped logs, queue lag, indexing latency, storage growth, and query latency.

### Trade-off
Full-text indexing improves search but costs storage and compute. Structured logs and targeted indexes are cheaper and often easier to operate.

---

## 5. Design a Notification System

### Requirements
Send email, SMS, push, or in-app notifications reliably at high volume.

### High-level architecture
Producer/service → notification API → validation/preferences/template service → durable queue → channel workers → external providers → delivery-status store.

### Data model
Notification ID, user ID, channel, template/payload, priority, status, retry count, idempotency key, created/sent timestamps.

### Request flow
1. Validate request and user preferences.
2. Persist notification/job and enqueue it.
3. Worker consumes job and sends via selected provider.
4. Record outcome; retry transient failures with exponential backoff and jitter.
5. Send permanent failures to a dead-letter queue and expose status.

### Reliability concerns
- Use idempotency keys to reduce duplicate sends.
- Exactly-once delivery is generally difficult across external providers; design for at-least-once processing and deduplication where possible.
- Apply provider quotas, backpressure, priority queues, and per-user rate limits.
- Respect opt-outs, privacy, and notification preferences.

---

## 6. Design a Task Scheduler

### Requirements
Run immediate or scheduled tasks, support priorities, retries, and potentially recurring jobs.

### Core components
- API for task submission/cancellation.
- Durable task store.
- Scheduler that identifies due tasks.
- Ready queue or priority queue.
- Worker pool.
- Status, retry, and dead-letter handling.

### Task record
Task ID, payload/reference, scheduled time, priority, status, attempt count, timeout, idempotency key.

### Flow
1. Store submitted task durably.
2. Scheduler finds tasks whose run time is due and enqueues them.
3. Workers claim tasks with a lease/visibility timeout.
4. Mark success, or retry transient failures with backoff.
5. Move exhausted failures to a dead-letter queue.

### Scaling and correctness
- Avoid relying on one in-memory heap as the only source of truth.
- Use leases/atomic claims so multiple workers do not normally run the same task simultaneously.
- Expect possible duplicate execution; make tasks idempotent.
- Partition by time or task key if needed; monitor queue age, execution latency, failures, and worker saturation.

---

## 7. Design a File System

### Requirements
Support creating directories/files, reading content, listing directories, and traversing paths. Clarify whether it is in-memory, local, or distributed.

### Simple in-memory design
- Represent directories as tree nodes.
- Each directory stores `name -> child node` in a hash map.
- File nodes store content or a reference to content.
- Parse paths into components and walk from the root.

### Example APIs
- `mkdir(path)`
- `createFile(path)`
- `writeFile(path, content)`
- `readFile(path)`
- `list(path)`

### Important cases
- Missing parent directory.
- Existing file/directory with the same name.
- Absolute versus relative paths.
- `.` and `..`, repeated slashes, permissions.
- Large files should use blocks/chunks rather than storing everything in memory.

### If distributed
Discuss metadata service, object/block storage, replication, consistency, permissions, quotas, recovery, and metadata caching. Do not claim the in-memory tree alone solves a distributed file system.

---

## 8. Design a Metrics / Monitoring System

### Requirements
Collect numeric time-series metrics, calculate aggregates, build dashboards, and alert when thresholds or conditions are met.

### High-level architecture
Applications/agents → ingestion endpoint → validation/aggregation → time-series storage → query API/dashboard + alert evaluator → alert delivery.

### Metric data
Metric name, timestamp, value, labels/tags (service, host, region, device).

### Key decisions
- Batch and compress writes.
- Partition data by time and metric/tenant.
- Choose retention and downsampling policies.
- Control high-cardinality labels; unbounded unique labels can make storage expensive.
- Use aggregation for dashboards and alerting windows.
- Avoid alert storms using grouping, deduplication, cooldowns, and severity.

### Monitoring the monitoring system
Track ingestion lag, dropped samples, query latency, storage capacity, alert evaluation lag, and notification failures.

---

## 9. Design a Distributed Cache

### Requirements
Serve frequently accessed data from memory across multiple nodes, with low latency and high availability.

### Main components
- Client/library or cache gateway.
- Cache nodes.
- Key partitioning/routing (consistent hashing is a common option).
- Eviction and TTL policy.
- Replication/failover mechanism.
- Optional backing database/source of truth.

### Request flow
1. Hash key to choose a cache node.
2. Check cache; on hit, return.
3. On miss, fetch from the source of truth, populate cache, and return.
4. Apply TTL/invalidation when data changes.

### Trade-offs and failure modes
- Cache-aside is simple but can have stale data and cache stampedes.
- TTL limits stale duration but does not provide immediate consistency.
- Use request coalescing, jittered TTLs, or locks to reduce stampedes.
- Decide replication consistency and what happens when a node fails.
- Avoid hot keys; consider sharding or replication.
- Never treat cache as the source of truth unless the product requirements explicitly allow it.

---

## 10. Design a Job Queue

### Requirements
Accept jobs, store them durably, let workers process them, retry failures, and report status.

### High-level architecture
Producer/API → durable queue or job store → worker pool → result/status store; failed jobs go to a dead-letter queue.

### Job states
`queued -> running -> succeeded` or `queued/running -> retry_wait -> queued`; after retry exhaustion, `dead_letter`.

### Important design choices
- At-least-once delivery is common; workers must be idempotent.
- Acknowledge only after safe processing, or use a lease/visibility timeout.
- Retry transient failures with exponential backoff and jitter.
- Use priorities/fairness if needed; avoid starvation.
- Support cancellation, timeouts, delayed jobs, and job status queries.
- Monitor queue depth, oldest-job age, throughput, retries, failures, and worker health.

### Job queue vs task scheduler
A job queue focuses on dispatching work to workers. A scheduler additionally decides **when** work becomes eligible to run, including future or recurring times.

---

# Part C — Quick comparison tables

## LRU vs LFU

| Feature | LRU | LFU |
|---|---|---|
| Eviction rule | Least recently used | Least frequently used |
| Extra tie-break | None needed | Usually LRU among same frequency |
| Typical structure | Hash map + doubly linked list | Hash maps + frequency buckets/list |
| O(1) average possible? | Yes | Yes, with suitable design |
| Main pitfall | Forgetting to update recency on `get` | Maintaining `minFrequency` and tie-breaking correctly |

## Queue, task scheduler, and job queue

| System | Primary job |
|---|---|
| Queue | Hold work until a consumer is ready |
| Task scheduler | Decide when work should run |
| Job queue | Persist and dispatch jobs, track state and retries |
| Worker pool | Execute claimed work concurrently |

## Common interview phrases worth saying
- “I’ll clarify the functional and non-functional requirements first.”
- “I’ll start with a simple correct design, then address scaling bottlenecks.”
- “The operation is O(1) average because hash-map operations are expected O(1).”
- “I’ll make workers idempotent because retries can lead to duplicate execution.”
- “I’ll use a durable queue to decouple producers from consumers and absorb traffic spikes.”
- “I’ll add metrics for latency, errors, throughput, queue lag, and resource saturation.”
- “I’ll state the consistency guarantees rather than assuming strong consistency.”

---

# Part D — Suggested study order

1. Min Stack — understand auxiliary state.
2. Top K Frequent Elements — hash map + heap.
3. Insert Delete GetRandom O(1) — vector + hash map.
4. Circular Queue — wrap-around indices.
5. Time Based Key-Value Store — binary search over history.
6. Copy List with Random Pointer — pointer mapping.
7. LRU Cache — hash map + doubly linked list.
8. Find Median from Data Stream — two heaps.
9. Rate limiter and job queue — simple service architecture.
10. Notification and logging systems — durable queues, retries, and backpressure.
11. Metrics/monitoring and distributed cache — data volume, partitioning, retention, consistency.
12. LFU cache and file system — more complex state and data modeling.

## Final interview checklist
For coding:
- Can I explain the chosen data structures before coding?
- Can I state the invariant that must remain true?
- Can I give time and space complexity?
- Have I checked empty input, duplicates, capacity limits, and pointer edge cases?

For system design:
- Did I clarify requirements and scale?
- Did I name the APIs and core data model?
- Did I explain the end-to-end flow?
- Did I cover concurrency, retries, idempotency, failures, and observability?
- Did I explain at least one trade-off?
