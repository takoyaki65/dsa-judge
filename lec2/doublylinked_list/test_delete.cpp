
#include "test_doublylinked_list.hpp"
#include <cstdio>
#include <unistd.h> // dup, dup2
#include <string>

// 先頭、末尾、中間のセルを削除できることを確認する
int main() {
  Cell *head = test_CreateCell(0, true);

  test_InsertNext(head, test_CreateCell(9, false));
  test_InsertNext(head, test_CreateCell(8, false));
  test_InsertNext(head, test_CreateCell(7, false));
  test_InsertNext(head, test_CreateCell(6, false));
  test_InsertNext(head, test_CreateCell(5, false));
  test_InsertNext(head, test_CreateCell(4, false));
  test_InsertNext(head, test_CreateCell(3, false));
  test_InsertNext(head, test_CreateCell(2, false));
  test_InsertNext(head, test_CreateCell(1, false));

  // list: 1 2 3 4 5 6 7 8 9

  // 先頭のセルを削除
  DeleteCell(head->next);
  // list: 2 3 4 5 6 7 8 9
  std::string output;
  test_Display(head, output, 256);
  if (output != "2 3 4 5 6 7 8 9 \n") {
    printf("Deletion on beginning failed.\n");
    exit(EXIT_FAILURE);
  }

  // 末尾のセルを削除
  DeleteCell(head->prev);
  // list: 2 3 4 5 6 7 8
  output.clear();
  test_Display(head, output, 256);
  if (output != "2 3 4 5 6 7 8 \n") {
    printf("Deletion on end failed.\n");
    exit(EXIT_FAILURE);
  }

  // 中間のセルを削除
  Cell *p = test_SearchCell(head, 5);
  DeleteCell(p);
  // list: 2 3 4 6 7 8
  output.clear();
  test_Display(head, output, 256);
  if (output != "2 3 4 6 7 8 \n") {
    printf("Deletion on middle failed.\n");
    exit(EXIT_FAILURE);
  }

  // どこにもつながれていないセルを削除したときに、エラーメッセージが表示されることを確認
  // 出力される標準エラー出力はpipeを使って文字列bufferに保存する
  char buffer[1024];
  // 標準出力エラーのfdを保存する
  int original_stderr_fd = dup(STDERR_FILENO);
  // 標準出力エラーを捕捉するためのpipeを作成
  int pipe_fd[2];
  pipe(pipe_fd);
  // 標準出力エラーをpipeのwrite endにリダイレクト
  dup2(pipe_fd[1], STDERR_FILENO);

  DeleteCell(test_CreateCell(0, false));

  // pipeから読み込み
  read(pipe_fd[0], buffer, sizeof(buffer) - 1);
  buffer[sizeof(buffer) - 1] = '\0'; // null-terminated

  // pipeを閉じる
  close(pipe_fd[0]);
  close(pipe_fd[1]);

  // 標準出力エラーを元に戻す
  dup2(original_stderr_fd, STDERR_FILENO);
  close(original_stderr_fd);

  if (std::string(buffer) != "This cell is not connected.\n") {
    printf("Error message is not displayed.\n");
    exit(EXIT_FAILURE);
  }

  printf("All tests passed.\n");
  return 0;
}
