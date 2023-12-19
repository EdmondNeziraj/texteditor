// #include <stdlib.h>
#include <string.h>

#include "../include/screen_container.h"

ScreenContainer::ScreenContainer()
{
    data_ = NULL;
    size_ = 0;
}

ScreenContainer::~ScreenContainer()
{
    delete[] data_;
    data_ = NULL;
    size_ = 0;
}

void ScreenContainer::Append(char *input, int input_length)
{
    char *new_data = (char *)realloc(this->data_, this->size_ + input_length);

    if (new_data == NULL)
        return;
    memcpy(&new_data[this->size_], input, input_length);
    this->data_ = new_data;
    this->size_ += input_length;
}

char *ScreenContainer::GetData()
{
    return data_;
}

int ScreenContainer::GetSize()
{
    return size_;
}