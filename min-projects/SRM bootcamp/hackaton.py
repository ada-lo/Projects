print("Welcome to the Retail Shopping Application - Level 2")
grand_total = 0
n=int(input("Enter the total no of entries :"))
for x in range (n):
    item_code = input("Enter item code: ")
    description = input("Enter item description: ")
    quantity = int(input("Enter quantity: "))
    price = float(input("Enter price per unit: "))
    total = quantity * price
    grand_total += total
    print("\nItem Summary:")
    print(f"Code: {item_code}")
    print(f"Description: {description}")
    print(f"Quantity: {quantity}")
    print(f"Price per unit: ${price:.2f}")
    print(f"Total cost: ${total:.2f}")
print(f"\nGrand Total for all items: ${grand_total:.2f}")
