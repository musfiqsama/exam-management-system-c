#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "exam.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#endif

void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen(void) {
    printf("\n%sPress Enter to continue...%s", YELLOW, RESET);
    getchar();
}

void slowPrint(const char *text, int delayMs) {
    int i;
    for (i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout);
#ifdef _WIN32
        Sleep(delayMs);
#else
        usleep(delayMs * 1000);
#endif
    }
}

void printLine(void) {
    printf("========================================================================\n");
}

void printBanner(void) {
    printLine();
    printf("%s%s                    EXAM MANAGEMENT SYSTEM%s\n", BOLD, CYAN, RESET);
    printLine();
}

void maskPasswordInput(char *password, int maxLen) {
    int i = 0;
#ifdef _WIN32
    char ch;
    while (1) {
        ch = _getch();
        if (ch == '\r') {
            password[i] = '\0';
            printf("\n");
            break;
        } else if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < maxLen - 1) {
            password[i++] = ch;
            printf("*");
        }
    }
#else
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((ch = getchar()) != '\n' && ch != EOF) {
        if ((ch == 127 || ch == 8) && i > 0) {
            i--;
        } else if (i < maxLen - 1 && ch != 127 && ch != 8) {
            password[i++] = (char)ch;
            printf("*");
            fflush(stdout);
        }
    }

    password[i] = '\0';
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
#endif
}

void getDataFilePath(const char *filename, char *outPath, int outSize) {
#ifdef _WIN32
    char exePath[PATH_LEN];
    DWORD len = GetModuleFileNameA(NULL, exePath, PATH_LEN);
    int i;

    if (len == 0 || len >= PATH_LEN) {
        snprintf(outPath, outSize, "%s", filename);
        return;
    }

    for (i = (int)len - 1; i >= 0; i--) {
        if (exePath[i] == '\\' || exePath[i] == '/') {
            exePath[i + 1] = '\0';
            break;
        }
    }

    snprintf(outPath, outSize, "%s%s", exePath, filename);
#else
    snprintf(outPath, outSize, "%s", filename);
#endif
}

void seedSampleQuestions(void) {
    char qpath[PATH_LEN];
    FILE *fp;

    getDataFilePath("questions.txt", qpath, PATH_LEN);

    fp = fopen(qpath, "r");
    if (fp != NULL) {
        fclose(fp);
        return;
    }

    fp = fopen(qpath, "w");
    if (fp == NULL) {
        printf("ERROR: Could not create questions file:\n%s\n", qpath);
        return;
    }

    fprintf(fp, "1|What is the correct extension of a C source file?|.cpp|.py|.c|.java|C|1|15|Because C source files are saved with the .c extension.\n");
    fprintf(fp, "2|Which function is used to print output in C?|scanf|printf|main|return|B|1|15|printf() is used to display output on the screen.\n");
    fprintf(fp, "3|Which header file is required for printf()?|string.h|math.h|stdio.h|conio.h|C|1|15|printf() is declared inside stdio.h.\n");
    fprintf(fp, "4|Which symbol is used to end a statement in C?|:|;|,|.|B|1|15|A semicolon ends a statement in C.\n");
    fprintf(fp, "5|Which keyword is used to define a constant in C?|fixed|const|define|static|B|1|15|const is used to declare a constant variable.\n");
    fprintf(fp, "6|Which operator is used for addition?|+|-|*|/|A|1|15|The plus sign is used for addition.\n");
    fprintf(fp, "7|Which function reads formatted input?|printf|scanf|puts|gets|B|1|15|scanf() reads formatted input from the keyboard.\n");
    fprintf(fp, "8|Which data type stores a single character?|int|float|char|string|C|1|15|char stores one single character.\n");
    fprintf(fp, "9|Which function is used to open a file?|fopen|open|read|close|A|1|15|fopen() is used to open a file in C.\n");
    fprintf(fp, "10|Which format specifier is used for integer?|%%f|%%d|%%c|%%s|B|1|15|%%d is used for integer values.\n");
    fprintf(fp, "11|Which loop runs at least once?|for|while|do-while|nested|C|1|15|do-while executes first, then checks condition.\n");
    fprintf(fp, "12|Which symbol dereferences a pointer?|&|*|%%|#|B|1|15|The * operator accesses the value stored at an address.\n");
    fprintf(fp, "13|Which keyword is used for decision making?|switch|goto|break|define|A|1|15|switch is a decision-making statement.\n");
    fprintf(fp, "14|Which operator checks equality?|=|==|!=|<=|B|1|15|== compares two values for equality.\n");
    fprintf(fp, "15|Which function is used to find string length?|strlen|strcpy|strcmp|strcat|A|1|15|strlen returns the length of a string.\n");
    fprintf(fp, "16|Which header is needed for string functions?|math.h|string.h|stdio.h|time.h|B|1|15|string.h contains string function declarations.\n");
    fprintf(fp, "17|What does malloc() do?|frees memory|allocates memory|prints memory|copies memory|B|1|15|malloc allocates memory dynamically.\n");
    fprintf(fp, "18|Which statement skips one loop iteration?|break|pass|continue|exit|C|1|15|continue skips the current iteration.\n");
    fprintf(fp, "19|What is the index of the first array element?|1|0|-1|2|B|1|15|Arrays in C start from index 0.\n");
    fprintf(fp, "20|Which operator gives address of a variable?|*|&|#|@|B|1|15|The & operator returns the memory address.\n");
    fprintf(fp, "21|Which keyword returns a value from a function?|break|return|void|case|B|1|15|return sends a value back to the calling function.\n");
    fprintf(fp, "22|What is recursion?|loop|function calling itself|array|pointer|B|1|15|Recursion means a function calls itself.\n");
    fprintf(fp, "23|Which format specifier is used for float?|%%d|%%c|%%f|%%s|C|1|15|%%f is used to print float values.\n");
    fprintf(fp, "24|Which function closes a file?|end|fclose|stop|done|B|1|15|fclose() closes an opened file.\n");
    fprintf(fp, "25|Which concept stores multiple values of same type?|pointer|array|function|loop|B|1|15|An array stores multiple elements of the same type.\n");
    fprintf(fp, "26|What is a NULL pointer?|random pointer|zero-valued pointer|char pointer|int pointer|B|1|15|A NULL pointer points to nothing valid.\n");
    fprintf(fp, "27|Which storage class keeps value between function calls?|auto|register|static|extern|C|1|15|static preserves value between function calls.\n");
    fprintf(fp, "28|Which function compares two strings?|strlen|strcmp|strcpy|strcat|B|1|15|strcmp compares two strings.\n");
    fprintf(fp, "29|Which header supports dynamic memory allocation?|stdlib.h|stdio.h|string.h|math.h|A|1|15|malloc, calloc, realloc, free are in stdlib.h.\n");
    fprintf(fp, "30|Which keyword is used to create a structure?|union|struct|enum|typedef|B|1|15|struct defines a user-defined structure.\n");

    fclose(fp);
}

int signupStudent(void) {
    char upath[PATH_LEN];
    Student s;
    FILE *fp;
    char line[256], existingName[NAME_LEN], existingUser[USERNAME_LEN], existingPass[PASSWORD_LEN];

    getDataFilePath("users.txt", upath, PATH_LEN);

    clearScreen();
    printBanner();
    printf("%sStudent Signup%s\n", GREEN, RESET);
    printLine();

    printf("Enter Full Name: ");
    readLine(s.name, sizeof(s.name));

    printf("Enter Username: ");
    readLine(s.username, sizeof(s.username));

    printf("Enter Password: ");
    maskPasswordInput(s.password, PASSWORD_LEN);

    fp = fopen(upath, "r");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            sscanf(line, "%59[^|]|%29[^|]|%29[^\n]", existingName, existingUser, existingPass);
            if (strcmp(existingUser, s.username) == 0) {
                fclose(fp);
                printf("%sUsername already exists!%s\n", RED, RESET);
                pauseScreen();
                return 0;
            }
        }
        fclose(fp);
    }

    fp = fopen(upath, "a");
    if (fp == NULL) {
        printf("%sCould not open user file.%s\n", RED, RESET);
        pauseScreen();
        return 0;
    }

    fprintf(fp, "%s|%s|%s\n", s.name, s.username, s.password);
    fclose(fp);

    printf("%sSignup successful!%s\n", GREEN, RESET);
    pauseScreen();
    return 1;
}

int loginStudent(char loggedInUser[]) {
    char upath[PATH_LEN];
    char username[USERNAME_LEN], password[PASSWORD_LEN];
    char line[256], fileName[NAME_LEN], fileUser[USERNAME_LEN], filePass[PASSWORD_LEN];
    FILE *fp;

    getDataFilePath("users.txt", upath, PATH_LEN);

    clearScreen();
    printBanner();
    printf("%sStudent Login%s\n", GREEN, RESET);
    printLine();

    printf("Enter Username: ");
    readLine(username, sizeof(username));

    printf("Enter Password: ");
    maskPasswordInput(password, PASSWORD_LEN);

    fp = fopen(upath, "r");
    if (fp == NULL) {
        printf("%sNo users found.%s\n", RED, RESET);
        pauseScreen();
        return 0;
    }

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%59[^|]|%29[^|]|%29[^\n]", fileName, fileUser, filePass);
        if (strcmp(fileUser, username) == 0 && strcmp(filePass, password) == 0) {
            strcpy(loggedInUser, username);
            fclose(fp);
            printf("%sLogin successful! Welcome, %s%s\n", GREEN, username, RESET);
            pauseScreen();
            return 1;
        }
    }

    fclose(fp);
    printf("%sInvalid username or password.%s\n", RED, RESET);
    pauseScreen();
    return 0;
}

int adminLogin(void) {
    char username[30], password[30];

    clearScreen();
    printBanner();
    printf("%sAdmin Login%s\n", MAGENTA, RESET);
    printLine();

    printf("Admin Username: ");
    readLine(username, sizeof(username));

    printf("Admin Password: ");
    maskPasswordInput(password, 30);

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        printf("%sAdmin login successful.%s\n", GREEN, RESET);
        pauseScreen();
        return 1;
    }

    printf("%sInvalid admin credentials.%s\n", RED, RESET);
    pauseScreen();
    return 0;
}

int loadQuestions(Question questions[]) {
    char qpath[PATH_LEN];
    FILE *fp;
    int count = 0;

    getDataFilePath("questions.txt", qpath, PATH_LEN);

    fp = fopen(qpath, "r");
    if (fp == NULL) {
        return 0;
    }

    while (count < MAX_QUESTIONS) {
        Question q;
        int matched = fscanf(
            fp,
            "%d|%299[^|]|%119[^|]|%119[^|]|%119[^|]|%119[^|]|%c|%d|%d|%299[^\n]\n",
            &q.id,
            q.question,
            q.optionA,
            q.optionB,
            q.optionC,
            q.optionD,
            &q.correct,
            &q.marks,
            &q.timeLimit,
            q.explanation
        );

        if (matched == 10) {
            questions[count++] = q;
        } else {
            break;
        }
    }

    fclose(fp);
    return count;
}

int saveAllQuestions(Question questions[], int count) {
    char qpath[PATH_LEN];
    FILE *fp;
    int i;

    getDataFilePath("questions.txt", qpath, PATH_LEN);

    fp = fopen(qpath, "w");
    if (fp == NULL) return 0;

    for (i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%s|%s|%s|%c|%d|%d|%s\n",
                questions[i].id,
                questions[i].question,
                questions[i].optionA,
                questions[i].optionB,
                questions[i].optionC,
                questions[i].optionD,
                questions[i].correct,
                questions[i].marks,
                questions[i].timeLimit,
                questions[i].explanation);
    }

    fclose(fp);
    return 1;
}

void addQuestion(void) {
    char qpath[PATH_LEN];
    Question q;
    FILE *fp;

    getDataFilePath("questions.txt", qpath, PATH_LEN);
    fp = fopen(qpath, "a");

    if (fp == NULL) {
        printf("%sCould not open question file.%s\n", RED, RESET);
        pauseScreen();
        return;
    }

    clearScreen();
    printBanner();
    printf("%sAdd Question%s\n", MAGENTA, RESET);
    printLine();

    printf("Question ID: ");
    if (scanf("%d", &q.id) != 1) {
        clearInputBuffer();
        fclose(fp);
        printf("%sInvalid ID input.%s\n", RED, RESET);
        pauseScreen();
        return;
    }
    clearInputBuffer();

    printf("Question: ");
    readLine(q.question, sizeof(q.question));

    printf("Option A: ");
    readLine(q.optionA, sizeof(q.optionA));

    printf("Option B: ");
    readLine(q.optionB, sizeof(q.optionB));

    printf("Option C: ");
    readLine(q.optionC, sizeof(q.optionC));

    printf("Option D: ");
    readLine(q.optionD, sizeof(q.optionD));

    printf("Correct Option (A/B/C/D): ");
    scanf(" %c", &q.correct);
    clearInputBuffer();
    if (q.correct >= 'a' && q.correct <= 'z') q.correct -= 32;

    q.marks = 1;

    printf("Time Limit (sec): ");
    if (scanf("%d", &q.timeLimit) != 1) {
        clearInputBuffer();
        fclose(fp);
        printf("%sInvalid time input.%s\n", RED, RESET);
        pauseScreen();
        return;
    }
    clearInputBuffer();

    printf("Explanation: ");
    readLine(q.explanation, sizeof(q.explanation));

    fprintf(fp, "%d|%s|%s|%s|%s|%s|%c|%d|%d|%s\n",
            q.id, q.question, q.optionA, q.optionB,
            q.optionC, q.optionD, q.correct, q.marks, q.timeLimit, q.explanation);

    fclose(fp);

    printf("%sQuestion added successfully.%s\n", GREEN, RESET);
    pauseScreen();
}

void editQuestion(void) {
    Question questions[MAX_QUESTIONS];
    int count = loadQuestions(questions);
    int id, i, found = 0;

    clearScreen();
    printBanner();
    printf("%sEdit Question%s\n", MAGENTA, RESET);
    printLine();

    if (count == 0) {
        printf("%sNo questions found.%s\n", RED, RESET);
        pauseScreen();
        return;
    }

    printf("Enter Question ID to edit: ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf("%sInvalid input.%s\n", RED, RESET);
        pauseScreen();
        return;
    }
    clearInputBuffer();

    for (i = 0; i < count; i++) {
        if (questions[i].id == id) {
            found = 1;

            printf("New Question: ");
            readLine(questions[i].question, sizeof(questions[i].question));

            printf("New Option A: ");
            readLine(questions[i].optionA, sizeof(questions[i].optionA));

            printf("New Option B: ");
            readLine(questions[i].optionB, sizeof(questions[i].optionB));

            printf("New Option C: ");
            readLine(questions[i].optionC, sizeof(questions[i].optionC));

            printf("New Option D: ");
            readLine(questions[i].optionD, sizeof(questions[i].optionD));

            printf("New Correct Option: ");
            scanf(" %c", &questions[i].correct);
            clearInputBuffer();
            if (questions[i].correct >= 'a' && questions[i].correct <= 'z') questions[i].correct -= 32;

            questions[i].marks = 1;

            printf("New Time Limit: ");
            if (scanf("%d", &questions[i].timeLimit) != 1) {
                clearInputBuffer();
                printf("%sInvalid time input.%s\n", RED, RESET);
                pauseScreen();
                return;
            }
            clearInputBuffer();

            printf("New Explanation: ");
            readLine(questions[i].explanation, sizeof(questions[i].explanation));
            break;
        }
    }

    if (!found) {
        printf("%sQuestion ID not found.%s\n", RED, RESET);
        pauseScreen();
        return;
    }

    saveAllQuestions(questions, count);
    printf("%sQuestion updated successfully.%s\n", GREEN, RESET);
    pauseScreen();
}

void deleteQuestion(void) {
    Question questions[MAX_QUESTIONS];
    int count = loadQuestions(questions);
    int id, i, j, found = 0;

    clearScreen();
    printBanner();
    printf("%sDelete Question%s\n", MAGENTA, RESET);
    printLine();

    if (count == 0) {
        printf("%sNo questions found.%s\n", RED, RESET);
        pauseScreen();
        return;
    }

    printf("Enter Question ID to delete: ");
    if (scanf("%d", &id) != 1) {
        clearInputBuffer();
        printf("%sInvalid input.%s\n", RED, RESET);
        pauseScreen();
        return;
    }
    clearInputBuffer();

    for (i = 0; i < count; i++) {
        if (questions[i].id == id) {
            found = 1;
            for (j = i; j < count - 1; j++) {
                questions[j] = questions[j + 1];
            }
            count--;
            break;
        }
    }

    if (!found) {
        printf("%sQuestion ID not found.%s\n", RED, RESET);
        pauseScreen();
        return;
    }

    saveAllQuestions(questions, count);
    printf("%sQuestion deleted successfully.%s\n", GREEN, RESET);
    pauseScreen();
}

void viewQuestions(void) {
    Question questions[MAX_QUESTIONS];
    int count = loadQuestions(questions);
    int i;

    clearScreen();
    printBanner();
    printf("%sQuestion Bank%s\n", BLUE, RESET);
    printLine();

    if (count == 0) {
        printf("%sNo questions available.%s\n", RED, RESET);
        pauseScreen();
        return;
    }

    for (i = 0; i < count; i++) {
        printf("ID: %d\n", questions[i].id);
        printf("Question: %s\n", questions[i].question);
        printf("A) %s\nB) %s\nC) %s\nD) %s\n",
               questions[i].optionA,
               questions[i].optionB,
               questions[i].optionC,
               questions[i].optionD);
        printf("Correct: %c | Marks: %d | Time: %d sec\n",
               questions[i].correct,
               questions[i].marks,
               questions[i].timeLimit);
        printf("Explanation: %s\n", questions[i].explanation);
        printLine();
    }

    pauseScreen();
}

void shuffleQuestions(Question questions[], int n) {
    int i;
    srand((unsigned)time(NULL));
    for (i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}

int inputWithTimeout(char *answer, int seconds) {
#ifdef _WIN32
    time_t start = time(NULL);
    int idx = 0;
    char ch;

    answer[0] = '\0';

    while (1) {
        int remaining = seconds - (int)difftime(time(NULL), start);
        if (remaining < 0) remaining = 0;

        printf("\rTime Left: %2d sec | Your answer: %s  ", remaining, answer);
        fflush(stdout);

        if (_kbhit()) {
            ch = _getch();

            if (ch == '\r') {
                answer[idx] = '\0';
                printf("\n");
                return 1;
            } else if (ch == '\b' && idx > 0) {
                idx--;
                answer[idx] = '\0';
            } else if (idx < 9 && ch != '\b') {
                answer[idx++] = ch;
                answer[idx] = '\0';
            }
        }

        if (difftime(time(NULL), start) >= seconds) {
            answer[0] = '\0';
            printf("\n");
            return 0;
        }

        Sleep(100);
    }
#else
    fd_set set;
    struct timeval timeout;
    int elapsed = 0;

    answer[0] = '\0';

    while (elapsed < seconds) {
        printf("\rTime Left: %2d sec | Type answer and press Enter: ", seconds - elapsed);
        fflush(stdout);

        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0) {
            if (fgets(answer, 10, stdin) != NULL) {
                answer[strcspn(answer, "\n")] = '\0';
                return 1;
            }
        }

        elapsed++;
    }

    answer[0] = '\0';
    printf("\n");
    return 0;
#endif
}

void takeExam(const char username[]) {
    Question allQuestions[MAX_QUESTIONS];
    char answer[10];
    int count, i;
    int totalQuestions = 0, correctAnswers = 0, wrongAnswers = 0, unanswered = 0;
    int examMarksChoice, questionCountToTake;
    double score = 0.0, negativeMark, maxPossibleMarks = 0.0, percentage;
    char grade[10];
    char rpath[PATH_LEN];
    FILE *fp;

    getDataFilePath("results.txt", rpath, PATH_LEN);

    clearScreen();
    printBanner();
    printf("%sTake Exam%s\n", GREEN, RESET);
    printLine();

    count = loadQuestions(allQuestions);
    if (count == 0) {
        printf("%sNo questions available.%s\n", RED, RESET);
        pauseScreen();
        return;
    }

    shuffleQuestions(allQuestions, count);

    printf("\nChoose Exam Marks:\n");
    printf("1. 10 Marks\n");
    printf("2. 20 Marks\n");
    printf("3. 30 Marks\n");
    printf("4. 40 Marks\n");
    printf("5. 50 Marks\n");
    printf("Enter choice: ");

    if (scanf("%d", &examMarksChoice) != 1) {
        clearInputBuffer();
        printf("%sInvalid input.%s\n", RED, RESET);
        pauseScreen();
        return;
    }
    clearInputBuffer();

    switch (examMarksChoice) {
        case 1: questionCountToTake = 10; break;
        case 2: questionCountToTake = 20; break;
        case 3: questionCountToTake = 30; break;
        case 4: questionCountToTake = 40; break;
        case 5: questionCountToTake = 50; break;
        default: questionCountToTake = 10;
    }

    if (questionCountToTake > count) {
        questionCountToTake = count;
    }

    printf("Enter negative marking per wrong answer (example 0 or 0.25 or 0.5): ");
    if (scanf("%lf", &negativeMark) != 1) {
        clearInputBuffer();
        printf("%sInvalid input.%s\n", RED, RESET);
        pauseScreen();
        return;
    }
    clearInputBuffer();

    clearScreen();
    printBanner();
    slowPrint("Exam is starting... Best of luck!\n", 20);
    printLine();
    printf("Total Questions in this Exam: %d\n", questionCountToTake);
    printLine();

    for (i = 0; i < questionCountToTake; i++) {
        char ch;

        totalQuestions++;
        maxPossibleMarks += allQuestions[i].marks;

        printf("\n%sQuestion %d%s\n", BOLD, totalQuestions, RESET);
        printf("%s\n", allQuestions[i].question);
        printf("A) %s\n", allQuestions[i].optionA);
        printf("B) %s\n", allQuestions[i].optionB);
        printf("C) %s\n", allQuestions[i].optionC);
        printf("D) %s\n", allQuestions[i].optionD);
        printf("Marks: %d | Time Limit: %d sec\n", allQuestions[i].marks, allQuestions[i].timeLimit);

        answer[0] = '\0';

        if (inputWithTimeout(answer, allQuestions[i].timeLimit)) {
            ch = answer[0];
            if (ch >= 'a' && ch <= 'z') ch -= 32;

            if (ch == allQuestions[i].correct) {
                printf("%sCorrect!%s\n", GREEN, RESET);
                printf("%sExplanation:%s %s\n", CYAN, RESET, allQuestions[i].explanation);
                correctAnswers++;
                score += allQuestions[i].marks;
            } else {
                printf("%sWrong! Correct answer: %c%s\n", RED, allQuestions[i].correct, RESET);
                printf("%sExplanation:%s %s\n", CYAN, RESET, allQuestions[i].explanation);
                wrongAnswers++;
                score -= negativeMark;
            }
        } else {
            printf("%sTime's up! No answer submitted.%s\n", RED, RESET);
            printf("%sExplanation:%s %s\n", CYAN, RESET, allQuestions[i].explanation);
            unanswered++;
        }

        printLine();
    }

    if (score < 0) score = 0;

    percentage = 0.0;
    if (maxPossibleMarks > 0) {
        percentage = (score / maxPossibleMarks) * 100.0;
    }

    if (percentage >= 80) strcpy(grade, "A+");
    else if (percentage >= 70) strcpy(grade, "A");
    else if (percentage >= 60) strcpy(grade, "B");
    else strcpy(grade, "Fail");

    fp = fopen(rpath, "a");
    if (fp != NULL) {
        fprintf(fp, "%s|%d|%d|%d|%d|%.2lf|%.2lf|%s\n",
                username, totalQuestions, correctAnswers, wrongAnswers,
                unanswered, score, percentage, grade);
        fclose(fp);
    }

    printf("\n%s%sEXAM FINISHED%s\n", BOLD, GREEN, RESET);
    printLine();
    printf("Student: %s\n", username);
    printf("Total Questions: %d\n", totalQuestions);
    printf("Correct: %d\n", correctAnswers);
    printf("Wrong: %d\n", wrongAnswers);
    printf("Unanswered: %d\n", unanswered);
    printf("Negative Mark per Wrong: %.2lf\n", negativeMark);
    printf("Final Score: %.2lf\n", score);
    printf("Percentage: %.2lf%%\n", percentage);
    printf("Grade: %s\n", grade);
    printLine();

    pauseScreen();
}

void viewMyResults(const char username[]) {
    char rpath[PATH_LEN];
    FILE *fp;
    char fileUser[USERNAME_LEN], grade[10];
    int totalQuestions, correctAnswers, wrongAnswers, unanswered;
    double score, percentage;
    int found = 0;

    getDataFilePath("results.txt", rpath, PATH_LEN);

    clearScreen();
    printBanner();
    printf("%sMy Result History%s\n", BLUE, RESET);
    printLine();

    fp = fopen(rpath, "r");
    if (fp == NULL) {
        printf("%sNo result file found.%s\n", RED, RESET);
        pauseScreen();
        return;
    }

    while (fscanf(fp, "%29[^|]|%d|%d|%d|%d|%lf|%lf|%9[^\n]\n",
                  fileUser, &totalQuestions, &correctAnswers, &wrongAnswers,
                  &unanswered, &score, &percentage, grade) == 8) {
        if (strcmp(fileUser, username) == 0) {
            found = 1;
            printf("Total: %d | Correct: %d | Wrong: %d | Unanswered: %d\n",
                   totalQuestions, correctAnswers, wrongAnswers, unanswered);
            printf("Score: %.2lf | Percentage: %.2lf%% | Grade: %s\n",
                   score, percentage, grade);
            printLine();
        }
    }

    fclose(fp);

    if (!found) {
        printf("%sNo results found.%s\n", RED, RESET);
    }

    pauseScreen();
}

void showLeaderboard(void) {
    char rpath[PATH_LEN];
    FILE *fp;
    Result results[MAX_RESULTS], temp;
    int count = 0, i, j;
    char grade[10];

    getDataFilePath("results.txt", rpath, PATH_LEN);

    clearScreen();
    printBanner();
    printf("%sLeaderboard%s\n", MAGENTA, RESET);
    printLine();

    fp = fopen(rpath, "r");
    if (fp == NULL) {
        printf("%sNo results found.%s\n", RED, RESET);
        pauseScreen();
        return;
    }

    while (fscanf(fp, "%29[^|]|%d|%d|%d|%d|%lf|%lf|%9[^\n]\n",
                  results[count].username,
                  &results[count].totalQuestions,
                  &results[count].correctAnswers,
                  &results[count].wrongAnswers,
                  &results[count].unanswered,
                  &results[count].score,
                  &results[count].percentage,
                  grade) == 8) {
        strcpy(results[count].grade, grade);
        count++;
        if (count >= MAX_RESULTS) break;
    }

    fclose(fp);

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (results[j].score > results[i].score) {
                temp = results[i];
                results[i] = results[j];
                results[j] = temp;
            }
        }
    }

    printf("%-5s %-20s %-12s %-12s\n", "Rank", "Username", "Score", "Grade");
    printLine();

    for (i = 0; i < count && i < 10; i++) {
        printf("%-5d %-20s %-12.2lf %-12s\n",
               i + 1, results[i].username, results[i].score, results[i].grade);
    }

    pauseScreen();
}

void showUserProfile(const char username[]) {
    char rpath[PATH_LEN];
    FILE *fp;
    char fileUser[USERNAME_LEN], grade[10];
    int totalQuestions, correctAnswers, wrongAnswers, unanswered;
    double score, percentage;
    int exams = 0;
    double totalScore = 0, bestScore = 0;

    getDataFilePath("results.txt", rpath, PATH_LEN);

    clearScreen();
    printBanner();
    printf("%sMy Profile%s\n", CYAN, RESET);
    printLine();

    fp = fopen(rpath, "r");
    if (fp == NULL) {
        printf("%sNo profile data found.%s\n", RED, RESET);
        pauseScreen();
        return;
    }

    while (fscanf(fp, "%29[^|]|%d|%d|%d|%d|%lf|%lf|%9[^\n]\n",
                  fileUser, &totalQuestions, &correctAnswers, &wrongAnswers,
                  &unanswered, &score, &percentage, grade) == 8) {
        if (strcmp(fileUser, username) == 0) {
            exams++;
            totalScore += score;
            if (score > bestScore) bestScore = score;
        }
    }

    fclose(fp);

    printf("Username: %s\n", username);
    printf("Total Exams Taken: %d\n", exams);
    if (exams > 0) {
        printf("Average Score: %.2lf\n", totalScore / exams);
        printf("Best Score: %.2lf\n", bestScore);
    } else {
        printf("Average Score: 0.00\n");
        printf("Best Score: 0.00\n");
    }

    printLine();
    pauseScreen();
}

void showAdminAnalytics(void) {
    char upath[PATH_LEN], rpath[PATH_LEN];
    FILE *fpUsers, *fpResults;
    char line[256], fileUser[USERNAME_LEN], grade[10];
    int userCount = 0, examCount = 0;
    int totalQuestions, correctAnswers, wrongAnswers, unanswered;
    double score, percentage, totalScore = 0;

    getDataFilePath("users.txt", upath, PATH_LEN);
    getDataFilePath("results.txt", rpath, PATH_LEN);

    clearScreen();
    printBanner();
    printf("%sAdmin Analytics%s\n", MAGENTA, RESET);
    printLine();

    fpUsers = fopen(upath, "r");
    fpResults = fopen(rpath, "r");

    if (fpUsers != NULL) {
        while (fgets(line, sizeof(line), fpUsers)) {
            userCount++;
        }
        fclose(fpUsers);
    }

    if (fpResults != NULL) {
        while (fscanf(fpResults, "%29[^|]|%d|%d|%d|%d|%lf|%lf|%9[^\n]\n",
                      fileUser, &totalQuestions, &correctAnswers, &wrongAnswers,
                      &unanswered, &score, &percentage, grade) == 8) {
            examCount++;
            totalScore += score;
        }
        fclose(fpResults);
    }

    printf("Total Registered Students: %d\n", userCount);
    printf("Total Exams Taken: %d\n", examCount);
    if (examCount > 0) {
        printf("Average Exam Score: %.2lf\n", totalScore / examCount);
    } else {
        printf("Average Exam Score: 0.00\n");
    }

    printLine();
    pauseScreen();
}
