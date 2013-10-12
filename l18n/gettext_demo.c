#include <locale.h>
#include <libintl.h>
#include <stdio.h>

#define _(string) gettext(string)

const char *DOMAIN  = "gettext_demo";
const char *DIRNAME = "languages"; 

int main(int argc, char **argv) {

  /* 设置locale与系统一致 */
  setlocale(LC_ALL, "");

  /* 添加域 gettext_test 在目录 po */
  bindtextdomain(DOMAIN, DIRNAME);

  /* 设定默认域为 gettext_test */
  textdomain(DOMAIN);

  /* gettext(Hello World) */
  puts(_("Hello"));
  puts(_("World"));
  printf(_("Welcome to use this!\n"));
  return 0;
}
