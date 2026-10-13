#include <stdio.h>
#include <math.h>

int main(void) {
    double a, b;
    int choice;

    printf("SuperLuxCalc C Console\n");
    printf("1:+  2:-  3:*  4:/  5:^  6:sqrt  7:sin  8:cos  9:tan  0:exit\n");

    while (1) {
        printf("\nChoose: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 0) {
            break;
        }

        if (choice >= 1 && choice <= 5) {
            printf("a b: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                break;
            }

            switch (choice) {
                case 1: printf("= %.10f\n", a + b); break;
                case 2: printf("= %.10f\n", a - b); break;
                case 3: printf("= %.10f\n", a * b); break;
                case 4:
                    if (b == 0.0) {
                        printf("Ошибка: деление на ноль\n");
                    } else {
                        printf("= %.10f\n", a / b);
                    }
                    break;
                case 5: printf("= %.10f\n", pow(a, b)); break;
            }
        } else if (choice == 6) {
            printf("a: ");
            if (scanf("%lf", &a) != 1) {
                break;
            }
            printf("= %.10f\n", sqrt(a));
        } else if (choice >= 7 && choice <= 9) {
            printf("a (в радианах): ");
            if (scanf("%lf", &a) != 1) {
                break;
            }
            switch (choice) {
                case 7: printf("= %.10f\n", sin(a)); break;
                case 8: printf("= %.10f\n", cos(a)); break;
                case 9: printf("= %.10f\n", tan(a)); break;
            }
        } else {
            printf("Неизвестная операция\n");
        }
    }

    return 0;
}
