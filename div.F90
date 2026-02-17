! Test OpenMP offload parallel do

module ex4_test
  implicit none

contains

  subroutine calc()
    real, dimension(10000) :: a, b, c

    integer :: i

    !$omp target parallel do num_threads(20)
    do i=1, 10000
      c(i) = a(i) / b(i)
    end do
    !$omp end target parallel do

  end subroutine calc

end module ex4_test

program main
  use ex4_test

implicit none

  call calc()
end program main
