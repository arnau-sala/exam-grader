# Lector - Exam Corrector with UPF System

An exam corrector developed from scratch in C that simulates the real optical mark recognition system used at UPF (Universitat Pompeu Fabra). The program reads answer sheets from external files and applies the university's unique two-row cancellation logic.

## Description

This project is an automatic test corrector that implements the real UPF answer sheet system:
- **Two-row system per question**: Upper row for answers, lower row for cancellation
- **Cancellation logic**: Mark the same option in both rows to cancel that choice
- **Error detection**: Multiple uncanceled options result in nullified questions
- **Penalty system**: Incorrect answers subtract points based on configurable penalty
- **External file reading**: Processes answer sheets from text files instead of hardcoded data

## Key Features

- **🎓 Real UPF System**: Implements the authentic two-row answer sheet format used at the university
- **❌ Cancellation Logic**: Students can cancel wrong answers by marking the lower row
- **📁 External Files**: Reads correction templates and student answers from text files
- **⚖️ Flexible Penalty**: Configurable penalty system (default: -1/4 per incorrect answer)
- **🔍 Smart Detection**: Automatically detects nullified, unanswered, and canceled questions
- **📊 Detailed Results**: Provides comprehensive scoring breakdown and statistics

## Project Structure

```
lector/
├── lector.c        # Main program
├── lector.h        # UPF interpretation logic and correction functions
├── macros.h        # Configuration (questions, options, penalty factor)
├── plantilla.txt   # Correct answers template (UPF format)
├── alumno.txt      # Student answer sheet (UPF format)
├── lector.exe      # Compiled executable
└── README.md       # This file
```

## Configuration

Modify settings in `macros.h`:
```c
#define PREGUNTES 12      // Number of exam questions
#define RESPOSTES 4       // Number of options per question (a, b, c, d)
#define PENALITZACIO 4    // Penalty factor (1/4 = -0.25 per error)
```

## UPF Answer Sheet Format

### Correct Answers Template (`plantilla.txt`)
Each line represents a question with marked correct options:
```
1: [X][ ][ ][X]    // Question 1: options a and d are correct
2: [ ][X][ ][ ]    // Question 2: option b is correct
3: [ ][X][ ][ ]    // Question 3: option b is correct
```

### Student Answer Sheet (`alumno.txt`)
Two lines per question - upper row for answers, lower row for cancellation:
```
1: [X][ ][ ][ ]    // Question 1: student marked option a
   [ ][ ][ ][ ]    // Question 1: no cancellations

2: [X][X][ ][ ]    // Question 2: student marked a and b initially
   [X][ ][ ][ ]    // Question 2: canceled option a
   // Final result: only option b remains active

3: [ ][ ][X][ ]    // Question 3: student marked option c
   [ ][ ][X][ ]    // Question 3: canceled option c
   // Final result: no answer (canceled)
```

## UPF System Logic

The program implements the authentic UPF answer sheet logic:

1. **Initial Marking**: Student marks options in the upper row
2. **Cancellation**: Student can cancel any option by marking the same position in the lower row
3. **Final Answer**: Only options marked in upper row but NOT in lower row count as active
4. **Multiple Options**: If multiple options remain active after cancellation → NULLIFIED
5. **No Options**: If all options are canceled → UNANSWERED

## Compilation and Execution

```bash
# Compile the program
gcc lector.c -o lector.exe

# Run the corrector
./lector.exe
```

## Scoring System

- **✅ Correct answer**: +1.00 points
- **❌ Incorrect answer**: -1/PENALITZACIO points (default: -0.25)
- **⚪ Unanswered**: 0.00 points  
- **🚫 Nullified** (multiple active options): 0.00 points
- **📝 Canceled** (marked then canceled): 0.00 points (treated as unanswered)
- **🎯 Final grade**: Sum of all questions (minimum 0.00), normalized to 10-point scale

## Example Output

```
question 1
    resposta: a    punts: 1.00
question 2
    ANULADA        punts: 0.00
question 3
    resposta: -    punts: 0.00

RESUM
    Correctes: 5
    Incorrectes: 2  
    No contestades: 3
    Anulades: 2

NF = 4.50 / 12
NF NORMALITZADA = 3.75
```

## What Makes This Project Unique

- **🏛️ Real University System**: Accurately simulates UPF's optical mark recognition
- **🔄 Cancellation Logic**: Implements the two-row system for error correction
- **📖 Educational Value**: Demonstrates file I/O, array manipulation, and complex logic
- **🎯 Practical Application**: Solves a real-world academic problem
- **🛠️ From Scratch**: Built entirely with custom logic, no external libraries

## Author

Project developed from scratch implementing the authentic UPF exam correction system with custom file processing and cancellation logic.