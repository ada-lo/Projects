# Display the sum and average of 3 numbers
 '''
print('Program to find sum and average of 3 numbers \n')
 
input_number1 = int(input('Enter the number1: '))
input_number2 = int(input('Enter the number2: '))
input_number3 = int(input('Enter the number3: '))
 
sum_number = input_number1 + input_number2 + input_number3
average_number1 = sum_number / 3  # floating point division
average_number2 = sum_number // 3 # Integer division
 
print(f'Sum of 3 numbers is {sum_number}')
print('Average of 3 numbers is', average_number1)
print('Average of 3 numbers is ' + str(average_number2))
 
 
# Program to check if a number is Even or Odd.
 
print('Enter a number to check if it is Even or Odd: ', end='')
 
input_number = int(input())
# The string returned by input() is converted (CASTING) to int
 
if input_number % 2 == 0:
    print('The number', input_number, ' is an Even number')
else:
    print('The number', input_number, ' is an Odd number')
 
# input_number   snake case
# inputNumber    camel case
 
# Program to check if a number is Even or not.
 
print('Enter a number to check if it is Even: ')
 
input_number = int(input())
# The string returned by input() is converted (CASTING) to int
 
print(type(input_number))
 
if input_number % 2 == 0:
    print('The number', input_number, ' is Even')
else:
    print('The number', input_number, ' is not Even')
 '''