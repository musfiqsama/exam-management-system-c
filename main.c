#include <stdio.h>
#include "exam.h"

int main(void) {
    int choice, studentChoice, adminChoice;
    char loggedInUser[USERNAME_LEN] = "";

    seedSampleQuestions();

    while (1) {
        clearScreen();
        printBanner();
        printf("1. Student Signup\n");
        printf("2. Student Login\n");
        printf("3. Admin Login\n");
        printf("4. Leaderboard\n");
        printf("5. Exit\n");
        printLine();
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid choice!\n");
            pauseScreen();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                signupStudent();
                break;

            case 2:
                if (loginStudent(loggedInUser)) {
                    while (1) {
                        clearScreen();
                        printBanner();
                        printf("Logged in as: %s\n", loggedInUser);
                        printLine();
                        printf("1. Take Exam\n");
                        printf("2. View My Results\n");
                        printf("3. My Profile\n");
                        printf("4. Logout\n");
                        printLine();
                        printf("Enter your choice: ");

                        if (scanf("%d", &studentChoice) != 1) {
                            clearInputBuffer();
                            printf("Invalid choice!\n");
                            pauseScreen();
                            continue;
                        }
                        clearInputBuffer();

                        if (studentChoice == 1) {
                            takeExam(loggedInUser);
                        } else if (studentChoice == 2) {
                            viewMyResults(loggedInUser);
                        } else if (studentChoice == 3) {
                            showUserProfile(loggedInUser);
                        } else if (studentChoice == 4) {
                            break;
                        } else {
                            printf("Invalid choice!\n");
                            pauseScreen();
                        }
                    }
                }
                break;

            case 3:
                if (adminLogin()) {
                    while (1) {
                        clearScreen();
                        printBanner();
                        printf("%sAdmin Panel%s\n", MAGENTA, RESET);
                        printLine();
                        printf("1. Add Question\n");
                        printf("2. Edit Question\n");
                        printf("3. Delete Question\n");
                        printf("4. View All Questions\n");
                        printf("5. Show Analytics\n");
                        printf("6. Logout\n");
                        printLine();
                        printf("Enter your choice: ");

                        if (scanf("%d", &adminChoice) != 1) {
                            clearInputBuffer();
                            printf("Invalid choice!\n");
                            pauseScreen();
                            continue;
                        }
                        clearInputBuffer();

                        if (adminChoice == 1) addQuestion();
                        else if (adminChoice == 2) editQuestion();
                        else if (adminChoice == 3) deleteQuestion();
                        else if (adminChoice == 4) viewQuestions();
                        else if (adminChoice == 5) showAdminAnalytics();
                        else if (adminChoice == 6) break;
                        else {
                            printf("Invalid choice!\n");
                            pauseScreen();
                        }
                    }
                }
                break;

            case 4:
                showLeaderboard();
                break;

            case 5:
                clearScreen();
                printf("%sThank you for using Exam Management System!%s\n", GREEN, RESET);
                return 0;

            default:
                printf("Invalid choice!\n");
                pauseScreen();
        }
    }

    return 0;
}
