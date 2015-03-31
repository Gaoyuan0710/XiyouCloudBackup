

#ifndef THREAD_POOL        //防止重复编译
#define THREAD_POOL

#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>


typedef struct Thread_work
{
    void * (*routine)(void *);    //线程函数
    void *arg;                    //线程函数的参数
    struct Thread_work *next;
}thread_work;

typedef struct 
{
    int is_destroy;                 //destroy为0时不销毁为1时销毁
    int max_thread_num;          //最大线程数
    pthread_t *thread_id;        //线程id数组指针
    thread_work *queue_head;     //工作队列头
    pthread_mutex_t queue_lock;   //对工作队列所上的锁
    pthread_cond_t queue_ready;  //唤醒等待线程的条件变量
}thread_pool;

#endif

thread_pool *tpool = NULL;
int thread_pool_create(int max_thread_num);
void thread_pool_destroy(void);
int pool_add_work(void *(*routine)(void *),void *arg);


//线程所执行的函数，主要负责从任务队列中取出任务并执行
void thread_routine(void)
{
    thread_work *work_t;          //用来存储所要执行的任务的指针

    while(1)
    {
        pthread_mutex_lock(&tpool->queue_lock);
        //如果任务队列为空，并且线程池未被销毁
        if(!tpool->queue_head && !tpool->is_destroy)
        {
            pthread_cond_wait(&tpool->queue_ready,&tpool->queue_lock);
        }

        if(tpool->is_destroy)
        {
            pthread_mutex_unlock(&tpool->queue_lock);
            pthread_exit(0);
        }

        work_t = tpool->queue_head;
        tpool->queue_head = work_t->next;
        pthread_mutex_unlock(&tpool->queue_lock);

        work_t->routine(work_t->arg);
        free(work_t);
    }
}

//创建线程池
int thread_pool_create(int max_thread_num)
{
    int i;

    tpool = (thread_pool *)malloc(sizeof(thread_pool));
    
    if(tpool == NULL)
    {
        printf("内存分配失败~!\n");
        exit(1);
    }

    //thread_pool 的初始化

    tpool->max_thread_num = max_thread_num;
    tpool->is_destroy = 0;
    tpool->queue_head = NULL;
    
    //初始化互斥锁
    if(pthread_mutex_init(&tpool->queue_lock,NULL) != 0)
    {
        printf("%d:pthread_mutex_init failed,error:%s",__LINE__,strerror(errno));
        exit(1);
    }
    
    //初始化条件变量
    if(pthread_cond_init(&tpool->queue_ready,NULL) != 0)
    {
        printf("%d:pthread_cond_init failed,errno:error:%s\n",__LINE__,strerror(errno));
        exit(1);
    }

    //为thread_id分配内存空间
    tpool->thread_id = (pthread_t *)malloc(max_thread_num * sizeof(pthread_t));
    
    if(tpool->thread_id == NULL)
    {
        printf("%d:malloc failed\n",__LINE__);
        exit(1);
    }

    for(i=0; i < max_thread_num;i++)
    {
        if(pthread_create(&tpool->thread_id[i],NULL,thread_routine,NULL) != 0)
        {
            printf("%d:pthread_create failed!\n",__LINE__);
            exit(1);
        }
    }
    return 0;
}


//销毁线程池
void thread_pool_destroy(void)
{
    int i;
    thread_work *work_t;

    if(tpool->is_destroy == 1)
    {
        return;
    }
    tpool->is_destroy = 1;    //当唤醒线程运行，遇到此状态线程就关闭

    //通知所有线程
    pthread_mutex_lock(&tpool->queue_lock);
    pthread_cond_broadcast(&tpool->queue_ready);
    pthread_mutex_unlock(&tpool->queue_lock);

    //此步为了确保所有线程均以退出
    for(i=0;i<tpool->max_thread_num;i++)
    {
        pthread_join(tpool->thread_id[i],NULL);
    }

    //释放线程id数组所占空间
    free(tpool->thread_id);
    //释放工作队列所占内存空间
    while(tpool->queue_head)
    {
        work_t = tpool->queue_head;
        tpool->queue_head = work_t->next;
        free(work_t);
    }
    //销毁锁，销毁条件变量
    pthread_mutex_destroy(&tpool->queue_lock);
    pthread_cond_destroy(&tpool->queue_ready);

    free(tpool);
    printf("销毁线程池成功!\n");
}

//给线程池分配任务
int pool_add_work(void *(*routine)(void *),void *arg)
{
    thread_work *work_t,*t;
    
    work_t = (thread_work *)malloc(sizeof(thread_work));
    if(!work_t)
    {
        printf("%d:malloc is failed!\n",__LINE__);
        return -1;
    }

    work_t->routine = routine;
    work_t->arg = arg;
    work_t->next = NULL;

    pthread_mutex_lock(&tpool->queue_lock);
    t = tpool->queue_head;
    if(!t)
    {
        tpool->queue_head = work_t;
    }
    else
    {
        while(t->next)
        {
            t = t->next;
        }
        t->next = work_t;
    }

    //通知处于阻塞状态工作线程有新任务添加
    pthread_cond_signal(&tpool->queue_ready);
    pthread_mutex_unlock(&tpool->queue_lock);
    return 0;
}
