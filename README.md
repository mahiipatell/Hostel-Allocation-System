# Hostel Allocation System

A merit-based hostel room allocation system built in C as a DSA semester project. Students are ranked by branch-normalized CGPA (z-score), then allocated rooms through an interactive terminal session with MIS-based identity verification, roommate preference selection, and a waiting list for quota overflow.

---

## Features

- **Z-score merit ranking** — CGPA is normalized per branch before ranking, so a 8.5 in Mechanical and a 8.5 in Computer are fairly compared across departments
- **Branch quota enforcement** — separate open and reserved seat counters per branch; quota is checked at both the main allocation phase and the waiting list phase
- **Roommate preference system** — higher-merit students pick a room and nominate two roommates by merit number; nominees see the invitation when their turn comes and can accept or pick elsewhere
- **Waiting list** — students who exceed branch quota are queued (FIFO) and allocated to remaining room capacity in a second pass
- **MIS verification** — 3-attempt identity check per student before allocation; admin override available for testing
- **Two-phase allocation** — main loop processes students in merit order, followed by a separate waiting list loop for overflow students

---

## Data Structures Used

| Structure | Role | Why |
|---|---|---|
| Doubly Linked List | Student roster (merit order) | Bidirectional traversal; efficient insert at end and node removal |
| Hash Table (chaining) | Room allocation (`fixed_rooms`, `hold_rooms`) | O(1) room lookup by room number; chaining handles multiple students per room |
| Stack × 6 | Branch + category quota counters | One stack per branch per category (open/reserved); full when `top == max_size - 1` |
| Queue | Waiting list | FIFO order preserved so earlier-merit students in the waitlist are served first |

---

## System Architecture

> Architecture diagram: `docs/pointer_diagram.drawio`  
> Open with [draw.io](https://app.diagrams.net) to view the full pointer diagram including hash table structure, DLL traversal, and stack/queue flows.

### Data flow

```
student_data.csv
      │
      ▼
Z-score sort (per branch mean + stddev)
      │
      ▼
sorted_students.csv
      │
      ▼
Doubly Linked List (merit order, head = rank 1)
      │
      ├──► Main allocation loop (merit order)
      │         │
      │         ├── Branch quota check (stack)
      │         ├── Hold room check (hold_rooms hash table)
      │         ├── Room selection → fixed_rooms hash table
      │         └── Quota full → waiting_list queue
      │
      └──► Waiting list loop
                │
                ├── Re-check branch quota
                ├── Pick from incomplete rooms
                └── Append to fixed_rooms
                      │
                      ▼
              allocated_rooms.csv
```

---

## Project Structure

```
hostel-allocation-system/
├── src/
│   ├── test.c                  # Main program — full allocation flow
│   ├── hash.c                  # Hash table (room allocation + hold system)
│   ├── linked_list.c           # Doubly linked list operations
│   ├── queue.c                 # Queue (waiting list)
│   ├── stack.c                 # Stack (branch/category quota counters)
│   ├── logistic.c              # Admin logistics input handler
│   ├── data_to_sorted_csv.c    # CSV reader + z-score ranker + sorter
│   └── sorted_csv_to_dll.c     # Sorted CSV to DLL loader
├── include/
│   ├── setting_up.h            # Core structs (student_node, student_node_dll)
│   ├── hash.h
│   ├── linked_list.h
│   ├── queue.h
│   └── stack.h
├── data/
│   └── student_data.csv        # Sample input (60 students, 3 branches)
├── docs/
│   └── pointer_diagram.drawio  # System architecture diagram
├── .gitignore
└── README.md
```

---

## How to Build and Run

**Requirements:** GCC, `-lm` flag (for `math.h`)

```bash
# Compile (run from the src/ directory)
gcc test.c hash.c linked_list.c queue.c stack.c logistic.c \
    data_to_sorted_csv.c sorted_csv_to_dll.c \
    -o hostel -lm

# Run
./hostel
```

**On Windows (MinGW/GCC):**
```bash
gcc test.c hash.c linked_list.c queue.c stack.c logistic.c ^
    data_to_sorted_csv.c sorted_csv_to_dll.c ^
    -o hostel.exe -lm
hostel.exe
```

---

## Input Format

**`student_data.csv`** — one student per line, no header row:
```
<serial>,<name>,<MIS>,<branch>,<category>,<cgpa>
-1,Siddharth Soman,612303004,computer,open,7.8
-1,Ravi Rao,612310001,mechanical,open,8.88
-1,Priya Nair,612320003,electrical,reserved,9.1
```

Valid values:
- `branch`: `computer`, `mechanical`, `electrical` (lowercase)
- `category`: `open`, `reserved` (lowercase)
- `cgpa`: float between 0.0 and 10.0

---

## Sample Admin Input (when prompted)

```
ADMIN ONLY
Enter the number of floors: 3
Enter the number of rooms on a floor: 20
Enter the total number of students: 60
Enter the total number of branches: 3
Enter the number of seats for Computer: 30
Enter the number of seats for Mechanical: 20
Enter the number of seats for Electrical: 10
Enter the reserved seats for Computer: 5
Enter the reserved seats for Mechanical: 4
Enter the reserved seats for Electrical: 2
```

Room numbers follow the pattern `X01–X20` where X is the floor number (e.g., Floor 1: rooms 101–120).

---

## Allocation Flow (Student's Perspective)

1. Students are called in merit order (rank 1 first)
2. Each student is asked if they want a hostel room
3. MIS number is verified (3 attempts; admin override: `1234`)
4. If a higher-merit student nominated them as a roommate, they see the room offer and can accept or decline
5. If no hold offer or declined, empty rooms are displayed and the student picks one
6. The student nominates 2 roommates by merit number (must be lower merit than theirs)
7. Allocation is written to `allocated_rooms.csv`

---

## Branches

| Branch | Description |
|---|---|
| `main` | Stable version — all 4 bugs fixed, clean structure |
| `optimization` | Iterative improvements — richer hold display (shows who nominated you), `append_to_hash` return value for error handling, admin MIS override |

---

## Known Limitations

- Room numbering is fixed to floors 1–3, rooms X01–X20 (not yet configurable at runtime)
- Branch names are case-sensitive (`computer`, not `Computer`)
- No persistent state — allocation resets on each run
- `student_data.csv` must be placed in the directory from which the executable is run
