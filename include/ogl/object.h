#ifndef OBJECT_H
#define OBJECT_H

#include "sb6mfile.h"

#ifndef SB6M_FILETYPES_ONLY

#include <GL/Glew.h>

namespace sb6
{
	
class Object
{
public:
    Object();
    ~Object();

    inline void render(unsigned int instance_count = 1,
                       unsigned int base_instance = 0)
    {
        render_sub_object(0, instance_count, base_instance);
    }

    void render_sub_object(unsigned int object_index,
                           unsigned int instance_count = 1,
                           unsigned int base_instance = 0);

    void get_sub_object_info(unsigned int index, GLuint &first, GLuint &count)
    {
        if (index >= num_sub_objects)
        {
            first = 0;
            count = 0;
        }
        else
        {
            first = sub_object[index].first;
            count = sub_object[index].count;
        }
    }

    unsigned int get_sub_object_count() const           { return num_sub_objects; }
    GLuint       get_vao() const                        { return vao; }
    void load(const char * filename);
    void free();

private:
    GLuint                  vertex_buffer;
    GLuint                  index_buffer;
    GLuint                  vao;
    GLuint                  num_indices;
    GLuint                  index_type;

    enum { MAX_SUB_OBJECTS = 256 };

    unsigned int            num_sub_objects;
    SB6M_SUB_OBJECT_DECL    sub_object[MAX_SUB_OBJECTS];
};

}
#endif /* SB6M_FILETYPES_ONLY */

#endif /* __OBJECT_H__ */

