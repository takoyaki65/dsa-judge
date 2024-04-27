#ifndef INCLUDE_GUARD_DOUBLYLINKED_LIST_IO_H
#define INCLUDE_GUARD_DOUBLYLINKED_LIST_IO_H
#include "doublylinked_list.h"

/**
 * @brief 配列dからn個の要素を読み込み、リストの先端に挿入する
 *        配列dの0番目の要素がリストの先頭になる
 * @param head リストのheadセル
 * @param d 読み込む配列
 * @param n 配列の要素数
 */
void ReadFromArray(Cell *head, int *d, int n);

/**
 * @brief リストの要素を配列dに書き込む
 *        リストの先頭が配列dの0番目になる
 * @param head リストのheadセル
 * @param d 書き込む配列
 * @param n 配列の要素数
 * @return 書き込んだ要素数
 */
int WriteToArray(Cell *head, int *d, int n);

/**
 * @brief ファイルからデータを読み込み、リストの先端に挿入する
 *        ファイルの最初に書かれたデータがリストの先頭になる
 * @param head リストのheadセル
 * @param fname ファイル名
 */
void ReadFromFile(Cell *head, const char *fname);

/**
 * @brief リストの要素をファイルに書き込む
 *        リストの先頭がファイルの最初に書かれる
 * @param head リストのheadセル
 * @param fname ファイル名
 */
void WriteToFile(Cell *head, const char *fname);

#endif // INCLUDE_GUARD_DOUBLYLINKED_LIST_IO_H