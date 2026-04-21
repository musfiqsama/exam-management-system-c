#ifndef EXAM_H
#define EXAM_H

#define MAX_USERS 200
#define MAX_QUESTIONS 500
#define MAX_RESULTS 1000

#define NAME_LEN 60
#define USERNAME_LEN 30
#define PASSWORD_LEN 30
#define QUESTION_LEN 300
#define OPTION_LEN 120
#define EXPLANATION_LEN 300
#define PATH_LEN 512

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define BOLD    "\x1b[1m"

typedef struct {
    char name[NAME_LEN];
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
} Student;

typedef struct {
    int id;
    char question[QUESTION_LEN];
    char optionA[OPTION_LEN];
    char optionB[OPTION_LEN];
    char optionC[OPTION_LEN];
    char optionD[OPTION_LEN];
    char correct;
    int marks;
    int timeLimit;
    char explanation[EXPLANATION_LEN];
} Question;

typedef struct {
    char username[USERNAME_LEN];
    int totalQuestions;
    int correctAnswers;
    int wrongAnswers;
    int unanswered;
    double score;
    double percentage;
    char grade[10];
} Result;

void clearScreen(void);
void pauseScreen(void);
void slowPrint(const char *text, int delayMs);
void printLine(void);
void printBanner(void);

void readLine(char *buffer, int size);
void clearInputBuffer(void);
void maskPasswordInput(char *password, int maxLen);

void getDataFilePath(const char *filename, char *outPath, int outSize);
void seedSampleQuestions(void);

int signupStudent(void);
int loginStudent(char loggedInUser[]);
int adminLogin(void);

int loadQuestions(Question questions[]);
int saveAllQuestions(Question questions[], int count);
void addQuestion(void);
void editQuestion(void);
void deleteQuestion(void);
void viewQuestions(void);

void shuffleQuestions(Question questions[], int n);
int inputWithTimeout(char *answer, int seconds);

void takeExam(const char username[]);
void viewMyResults(const char username[]);
void showLeaderboard(void);
void showUserProfile(const char username[]);
void showAdminAnalytics(void);

#endif
