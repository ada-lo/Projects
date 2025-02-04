avg=0
total=0
name=input("Enter name of student:")
for x in range (3): 
    marks=int(input("Enter marks of subject:"))
    total+=marks
avg=total/3
print("total the is ", total)
print("the avg is ", avg)
if avg <=60:
    print(f'{avg:,.2f} First Class')
elif avg <=50:
    print(f'{avg:,.2f} Second Class')
elif avg <=35:
    print(f'{avg:,.2f} Pass Class')
else
    print(f'{avg:,.2f} FAIL')


