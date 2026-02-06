#include "StringList.h"
#define NULL 0

int main() {
    char** list = NULL;
    StringListInit(&list);  
    if (!list) {
        return 1;
    }

    StringListAdd(list, "Mercury");  
    StringListAdd(list, "Mars");
    StringListAdd(list, "Earth");
    StringListAdd(list, "Mars");
    StringListAdd(list, "Jupiter");
    
    if (StringListSize(list) != 5) {
        StringListDestroy(&list);  
        return 2;
    }
    if (StringListIndexOf(list, "Mars") != 1) {
        StringListDestroy(&list);
        return 3;
    }

    StringListRemove(list, "Mars"); 
    if (StringListSize(list) != 3) {
        StringListDestroy(&list);
        return 4;
    }

    const char* s = "Saturn";
    StringListAdd(list, s);  
    StringListAdd(list, "Mercury");  

    if (StringListSize(list) != 5) {
        StringListDestroy(&list);
        return 5;
    }

    StringListRemoveDuplicates(list);  
    if (StringListSize(list) != 4) {
        StringListDestroy(&list);
        return 6;
    }

    StringListReplaceInStrings(list, "ar", "AR");
    if (StringListIndexOf(list, "EARth") == -1) {
        StringListDestroy(&list);
        return 7;
    }

    StringListSort(list);

    StringListDestroy(&list);  
    if (list != NULL) {
        return 8;
    }

    return 0;
}