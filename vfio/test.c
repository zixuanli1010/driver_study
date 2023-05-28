#include <stdio.h>
#include <errno.h>

int main() {
    FILE *file = fopen("nonexistent.txt", "r");
    if (file == NULL) {
        perror("Error:jslkdjfl");
        return errno;
    }
    
    // 继续进行其他操作...
    
    fclose(file);
    return 0;
}
