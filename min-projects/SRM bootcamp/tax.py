empname = input('enter employee name:')
sal=float(input("enter salary:"))
if sal > 300000:
    print(f"{empname} should pay the salary")
else:
    print(f"{empname} shouldnt pay the salary")