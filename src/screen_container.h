#ifndef SCREEN_CONTAINER_H
#define SCREEN_CONTAINER_H

class ScreenContainer
{
private:
    char *data_;
    int size_;

public:

    /**
     * @brief Construct a new screen container object.
     * 
     */
    ScreenContainer();

    /**
     * @brief Destroy the screen container object.
     * 
     */
    ~ScreenContainer();

    /**
     * @brief Append new data in the screen container.
     *
     * @param input is the char array to be appended.
     * @param input_length is the length of the input.
     */
    void Append(char *input, int input_length);

    /**
     * @brief Get the content of the of the container.
     * 
     * @return char* data_, a pointer to the start of the char array.
     */
    char *GetData();

    /**
     * @brief Get the size of the data_ char array.
     * 
     * @return int size_, the size of the data_ array.
     */
    int GetSize();
};

#endif
