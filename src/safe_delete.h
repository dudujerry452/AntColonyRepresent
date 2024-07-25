#ifndef SAFE_DELETE_H
#define SAFE_DELETE_H

namespace safe_delete{
template <class T>
void SafeDelete(T *&t){
    if(t) {
        delete t;
        t = nullptr;
    }
}

template <class T>
void SafeDeleteArray(T *&t){
    if(t) {
        delete[]t;
        t = nullptr;
    }
}

template <class T>
void SafeDeleteNDimensionalArray(T *&t,int num_ndim) {
    if(t){
        for (int i = 0; i < num_ndim; ++i) {
            SafeDeleteArray(t[i]);
        }
        SafeDeleteArray(t);
    }
}
}


#endif // SAFE_DELETE_H
