/**
 * @file circular_buffer.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "circular_buffer.h"

//*****************************************************************************
// Initializes a circular buffer.
//
// Parameters
//    max_size:   Number of entries in the circular buffer.
//*****************************************************************************
Circular_Buffer * circular_buffer_init(uint16_t max_size)
{
   /* Allocate memory from the heap fro the circular buffer struct */
   Circular_Buffer *buffer = malloc(sizeof(Circular_Buffer));

   /* Allocate memory from the heap that will be used to store the characters/data
    * in the circular buffer
    */
   buffer->data = malloc(max_size);

   /* Initialize the max_size, produce count, and consume count*/ 
   buffer->max_size = max_size;
   buffer->produce_count = 0;
   buffer->consume_count = 0;

   /* Return a pointer to the circular buffer data structure */
   return buffer;
}

//*****************************************************************************
// Returns a circular buffer to the heap
//
// Parameters
//    buffer  :   The address of the circular buffer.
//*****************************************************************************
void circular_buffer_delete(Circular_Buffer * buffer)
{
   free((void *)buffer->data);
   free(buffer);
}

//*****************************************************************************
// Returns true if the circular buffer is empty.  Returns false if it is not.
//
// Parameters
//    buffer  :   The address of the circular buffer.
//*****************************************************************************
bool circular_buffer_empty(Circular_Buffer *buffer)
{
  /* ADD CODE */
  return (buffer->produce_count - buffer->consume_count == 0);
  /* Use the buffer->produce_count and buffer->consume count to determine if 
   * the circular buffer is empty 
   */
}

//*****************************************************************************
// Returns true if the circular buffer is full.  Returns false if it is not.
//
// Parameters
//    buffer  :   The address of the circular buffer.
//*****************************************************************************
bool circular_buffer_full(Circular_Buffer *buffer)
{
  /* ADD CODE */
  return buffer->produce_count - buffer->consume_count == buffer->max_size;
  /* Use the buffer->produce_count and buffer->consume count to determine if 
   * the circular buffer is full
   */

}
//*****************************************************************************
// Adds a character to the circular buffer.
//
// Parameters
//    buffer  :   The address of the circular buffer.
//    c       :   Character to add.
//*******************************************************************************
bool circular_buffer_add(Circular_Buffer *buffer, char c)
{
  /* ADD CODE */

  // Use the function defined above to determine if the circular buffer is full
  // If the circular buffer is full, return false.
  if (circular_buffer_full(buffer)) {
    return false;
  }
  // Add the data to the circular buffer.  
  buffer->data[buffer->produce_count % buffer->max_size] = c;
  buffer->produce_count++;
  // Return true to indicate that the data was added to the
  // circular buffer.
  return true;

}

//*****************************************************************************
// Removes the oldest character from the circular buffer.
//
// Parameters
//    buffer  :   The address of the circular buffer.
//
// Returns
//    The character removed from the circular buffer.  If the circular buffer
//    is empty, the value of the character returned is set to 0.
//*****************************************************************************
char circular_buffer_remove(Circular_Buffer *buffer)
{
  char return_char;

  /* ADD CODE */

  // If the circular buffer is empty, return 0.
  // Use the function defined above to determine if the circular buffer is empty
  if (circular_buffer_empty(buffer)) {
    return 0;
  }
  // remove the character from the circular buffer
  return_char = buffer->data[buffer->consume_count % buffer->max_size];
  buffer->consume_count++;
  // return the character
  return return_char;

}

