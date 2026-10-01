#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <windows.h>

struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

struct Node *root = NULL;

struct Node* create_node(int data) {
    struct Node *p = (struct Node*)malloc(sizeof(struct Node));
    if (p == NULL) {
        printf("[Ошибка] Не удалось выделить динамическую память под узел!\n");
        exit(1);
    }
    p->data = data;
    p->left = NULL;
    p->right = NULL;
    return p;
}

struct Node* insert_node(struct Node *r, int data) {
    if (r == NULL) {
        return create_node(data);
    }
    if (data < r->data) {
        r->left = insert_node(r->left, data);
    } else {
        r->right = insert_node(r->right, data);
    }
    return r;
}

struct Node* CreateTree(struct Node *treeRoot, struct Node *r, int data) {
    if (r == NULL) {
        struct Node *p = create_node(data);
        if (treeRoot == NULL) return p;
        if (data < treeRoot->data) treeRoot->left = p;
        else treeRoot->right = p;
        return p;
    }
    if (data < r->data) {
        if (r->left == NULL) {
            r->left = CreateTree(treeRoot, r->left, data);
        } else {
            CreateTree(treeRoot, r->left, data);
        }
    } else {
        if (r->right == NULL) {
            r->right = CreateTree(treeRoot, r->right, data);
        } else {
            CreateTree(treeRoot, r->right, data);
        }
    }
    return treeRoot;
}

struct Node* search_tree(struct Node *r, int key, int *comparisons) {
    if (r == NULL) {
        return NULL;
    }
    (*comparisons)++;
    if (key == r->data) {
        return r;
    }
    if (key < r->data) {
        return search_tree(r->left, key, comparisons);
    } else {
        return search_tree(r->right, key, comparisons);
    }
}

int count_occurrences(struct Node *r, int key) {
    if (r == NULL) {
        return 0;
    }
    int count = 0;
    if (r->data == key) {
        count = 1;
    }
    if (key < r->data) {
        count += count_occurrences(r->left, key);
    } else if (key > r->data) {
        count += count_occurrences(r->right, key);
    } else {
        count += count_occurrences(r->right, key);
    }
    return count;
}

struct Node* insert_unique(struct Node *r, int data, int *added) {
    if (r == NULL) {
        *added = 1;
        return create_node(data);
    }
    if (data == r->data) {
        *added = 0;
        printf("[Исключение дубликата] Элемент %d уже существует в дереве! Добавление отклонено.\n", data);
        return r;
    }
    if (data < r->data) {
        r->left = insert_unique(r->left, data, added);
    } else {
        r->right = insert_unique(r->right, data, added);
    }
    return r;
}

void print_tree(struct Node *r, int l) {
    if (r == NULL) {
        return;
    }
    print_tree(r->right, l + 1);
    for (int i = 0; i < l; i++) {
        printf("    ");
    }
    printf("%d\n", r->data);
    print_tree(r->left, l + 1);
}

void free_tree(struct Node *r) {
    if (r == NULL) return;
    free_tree(r->left);
    free_tree(r->right);
    free(r);
}

int count_nodes(struct Node *r) {
    if (r == NULL) return 0;
    return 1 + count_nodes(r->left) + count_nodes(r->right);
}

int tree_height(struct Node *r) {
    if (r == NULL) return 0;
    int h_left = tree_height(r->left);
    int h_right = tree_height(r->right);
    return 1 + (h_left > h_right ? h_left : h_right);
}
//1. Найти начальный и последний уровень вхождения искомых элементов
//2. Удаление узла заданного(в дереве не содержащим повторений)
void find_levels(struct Node *r, int key, int level, int *min_lvl, int *max_lvl, int *count) {
    if (r == NULL) {
        return;
    }
    if (r->data == key) {
        (*count)++;
        if (*min_lvl == -1 || level < *min_lvl) {
            *min_lvl = level;
        }
        if (*max_lvl == -1 || level > *max_lvl) {
            *max_lvl = level;
        }
    }
    if (key <= r->data) {
        find_levels(r->left, key, level + 1, min_lvl, max_lvl, count);
    }
    if (key >= r->data) {
        find_levels(r->right, key, level + 1, min_lvl, max_lvl, count);
    }
}

struct Node* min_node(struct Node *r) {
    while (r != NULL && r->left != NULL) {
        r = r->left;
    }
    return r;
}

struct Node* delete_node(struct Node *r, int key, int *deleted) {
    if (r == NULL) {
        return NULL;
    }
    if (key < r->data) {
        r->left = delete_node(r->left, key, deleted);
    } else if (key > r->data) {
        r->right = delete_node(r->right, key, deleted);
    } else {
        *deleted = 1;
        if (r->left == NULL) {
            struct Node *temp = r->right;
            free(r);
            return temp;
        } else if (r->right == NULL) {
            struct Node *temp = r->left;
            free(r);
            return temp;
        }
        struct Node *temp = min_node(r->right);
        r->data = temp->data;
        int dummy = 0;
        r->right = delete_node(r->right, temp->data, &dummy);
    }
    return r;
}

int main(void) {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    int choice = -1;
    int val = 0;
    int comps = 0;
    int added = 0;
    struct Node *found = NULL;

    while (choice != 0) {
        printf("\n========================================================\n");
        printf("  ЛАБОРАТОРНАЯ РАБОТА № 4: БИНАРНОЕ ДЕРЕВО ПОИСКА       \n");
        printf("========================================================\n");
        printf(" 1. Построить дерево вручную (ввод чисел, конец — символ)\n");
        printf(" 2. Добавить элемент с проверкой уникальности (Задание 3*)\n");
        printf(" 3. Поиск элемента в дереве с подсчетом шагов (Задание 1)\n");
        printf(" 4. Подсчет числа вхождений заданного значения (Задание 2)\n");
        printf(" 5. Вывести графическое представление дерева (на 90°)\n");
        printf(" 6. Найти начальный и последний уровень вхождения (Доп. задание 1)\n");
        printf(" 7. Удаление узла в дереве без повторений (Доп. задание 2)\n");
        printf(" 0. Выход из программы\n");
        printf("--------------------------------------------------------\n");
        printf("Выберите пункт меню: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Некорректный ввод! Введите число от 0 до 7.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("\n--- Построение дерева вручную по методичке ---\n");
                printf("Вводите целые числа (любой символ завершает ввод):\n");
                while (1) {
                    printf("Введите число: ");
                    if (scanf("%d", &val) != 1) {
                        while (getchar() != '\n');
                        printf("Построение дерева окончено.\n\n");
                        break;
                    }
                    root = insert_node(root, val);
                }
                printf("Текущее дерево:\n");
                print_tree(root, 0);
                break;

            case 2:
                printf("Введите число для добавления (с защитой от дубликатов): ");
                if (scanf("%d", &val) == 1) {
                    added = 0;
                    root = insert_unique(root, val, &added);
                    if (added) {
                        printf("[Успех] Элемент %d успешно добавлен в дерево.\n", val);
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Ошибка ввода!\n");
                }
                break;

            case 3:
                if (root == NULL) {
                    printf("Дерево пусто! Сначала добавьте элементы.\n");
                    break;
                }
                printf("Введите число для поиска: ");
                if (scanf("%d", &val) == 1) {
                    comps = 0;
                    found = search_tree(root, val, &comps);
                    if (found != NULL) {
                        printf("[Результат] Элемент %d НАЙДЕН! (Количество сравнений: %d)\n", val, comps);
                    } else {
                        printf("[Результат] Элемент %d НЕ НАЙДЕН в дереве! (Количество сравнений: %d)\n", val, comps);
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Ошибка ввода!\n");
                }
                break;

            case 4:
                if (root == NULL) {
                    printf("Дерево пусто! Сначала добавьте элементы.\n");
                    break;
                }
                printf("Введите число для подсчета вхождений: ");
                if (scanf("%d", &val) == 1) {
                    int cnt = count_occurrences(root, val);
                    printf("[Результат] Элемент %d встречается в дереве: %d раз(а).\n", val, cnt);
                } else {
                    while (getchar() != '\n');
                    printf("Ошибка ввода!\n");
                }
                break;

            case 5:
                if (root == NULL) {
                    printf("Дерево пусто!\n");
                } else {
                    printf("\nДерево (повернуто на 90 градусов, корень слева):\n\n");
                    print_tree(root, 0);
                    printf("\n");
                }
                break;

            case 6:
                if (root == NULL) {
                    printf("Дерево пусто! Сначала добавьте элементы.\n");
                    break;
                }
                printf("Введите число для поиска уровней вхождения: ");
                if (scanf("%d", &val) == 1) {
                    int min_lvl = -1;
                    int max_lvl = -1;
                    int cnt = 0;
                    find_levels(root, val, 0, &min_lvl, &max_lvl, &cnt);
                    if (cnt > 0) {
                        printf("[Результат] Элемент %d найден (вхождений: %d):\n", val, cnt);
                        printf("  Начальный уровень вхождения: %d\n", min_lvl);
                        printf("  Последний уровень вхождения:  %d\n", max_lvl);
                        printf("  (уровень корня: 0)\n");
                    } else {
                        printf("[Результат] Элемент %d НЕ НАЙДЕН в дереве!\n", val);
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Ошибка ввода!\n");
                }
                break;

            case 7:
                if (root == NULL) {
                    printf("Дерево пусто! Сначала добавьте элементы.\n");
                    break;
                }
                printf("Введите число для удаления: ");
                if (scanf("%d", &val) == 1) {
                    int deleted = 0;
                    root = delete_node(root, val, &deleted);
                    if (deleted) {
                        printf("[Успех] Узел %d успешно удален из дерева.\n", val);
                        printf("Текущее дерево:\n");
                        print_tree(root, 0);
                    } else {
                        printf("[Результат] Узел %d НЕ НАЙДЕН в дереве!\n", val);
                    }
                } else {
                    while (getchar() != '\n');
                    printf("Ошибка ввода!\n");
                }
                break;

            case 0:
                printf("Освобождение памяти и выход из программы...\n");
                free_tree(root);
                root = NULL;
                printf("Программа успешно завершена.\n");
                break;

            default:
                printf("Неверный выбор! Пожалуйста, выберите пункт от 0 до 7.\n");
                break;
        }
    }

    return 0;
}
