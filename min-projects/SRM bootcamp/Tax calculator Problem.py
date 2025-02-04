name = input("Enter Employee Name: ")
emp_id = input("Enter Employee ID: ")
gross_monthly_salary = float(input("Enter Gross Monthly Salary (₹): "))
bonus = float(input("Enter Annual Bonus (₹): "))

gross_annual_salary = (gross_monthly_salary * 12) + bonus

print("\n--- Employee Details ---")
print(f"Name                : {name}")
print(f"Employee ID         : {emp_id}")
print(f"Gross Monthly Salary: ₹{gross_monthly_salary:,.2f}")
print(f"Annual Bonus        : ₹{bonus:,.2f}")
print(f"Annual Gross Salary : ₹{gross_annual_salary:,.2f}")

standard_deduction = 50000
taxable_income = gross_annual_salary - standard_deduction

print("\n--- Taxable Income Calculation ---")
print(f"Standard Deduction  : ₹{standard_deduction:,.2f}")
print(f"Taxable Income      : ₹{taxable_income:,.2f}")

tax = 0
if taxable_income > 1500000:
    tax += (taxable_income - 1500000) * 0.30
    taxable_income = 1500000
if taxable_income > 1200000:
    tax += (taxable_income - 1200000) * 0.20
    taxable_income = 1200000
if taxable_income > 900000:
    tax += (taxable_income - 900000) * 0.15
    taxable_income = 900000
if taxable_income > 600000:
    tax += (taxable_income - 600000) * 0.10
    taxable_income = 600000
if taxable_income > 300000:
    tax += (taxable_income - 300000) * 0.05

if taxable_income <= 700000:
    tax = 0

cess = tax * 0.04
total_tax = tax + cess

print("\n--- Tax Calculation ---")
print(f"Tax (before cess)   : ₹{tax:,.2f}")
print(f"Health & Edu. Cess  : ₹{cess:,.2f}")
print(f"Total Tax Payable   : ₹{total_tax:,.2f}")

net_annual_salary = gross_annual_salary - total_tax

print("\n--- Net Salary Calculation ---")
print(f"Annual Gross Salary : ₹{gross_annual_salary:,.2f}")
print(f"Total Tax Payable   : ₹{total_tax:,.2f}")
print(f"Annual Net Salary   : ₹{net_annual_salary:,.2f}")
