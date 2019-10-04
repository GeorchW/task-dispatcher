#include <iostream>
#include <td.hh>

namespace
{
void argfun(int a, int b, int c) { printf("Argfun: %d %d %d \n", a, b, c); }

struct Foo
{
    void process_args(int a, int b) { printf("Foo process %d %d \n", a, b); }
};

template <class F, class FObj, class... Args>
void execute(F&& func, FObj& inst, Args&&... args)
{
    (inst.*func)(args...);
}

}

int main()
{
    td::launch([] {
        auto s1 = td::submit([] { printf("Task 1\n"); });
        td::submit(s1, [] { printf("Task 1 - append \n"); });
        td::submit(s1, argfun, 1, 2, 3);

        // TODO
        //        Foo f;
        //        td::submit(s1, &Foo::process_args, f, 15, 16);

        td::wait_for_unpinned(s1);

        auto future1 = td::submit([] { return 5.f * 15.f; });
        future1.get_unpinned();

        td::sync s2;

        td::submit(s2, [] {
            printf("Task 2 start \n");

            auto s2_i = td::submit_n([](auto i) { printf("Task 2 - inner %d \n", i); }, 4);

            td::wait_for_unpinned(s2_i);

            printf("Task 2 end \n");
        });

        td::submit(s2, [] { printf("Task 3 \n"); });

        td::wait_for_unpinned(s2);

        td::sync s3;
        td::submit_n(
            s3,
            [](auto i) {
                printf("Task 4 - %d start \n", i);

                auto s4_i = td::submit_n(
                    [i](auto i_inner) {
                        //
                        printf("Task 4 - %d - inner %d \n", i, i_inner);
                    },
                    4);

                printf("Task 4 - %d wait \n", i);
                td::wait_for_unpinned(s4_i);
            },
            4);

        td::wait_for_unpinned(s3);
    });


    return 0;
}
