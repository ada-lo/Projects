def sal_and_real(crops, chemical_free_acres, acres_per_segment):
    results = {}
    total_sales = 0
    for crop, values in crops.items():
        yield_per_acre, price_per_tonne = values
        total_yield = yield_per_acre * acres_per_segment
        total_sales += total_yield * price_per_tonne
        results[f"{crop}_sales"] = total_yield * price_per_tonne

    chemical_free_realization = 0
    for crop, free_acres in chemical_free_acres.items():
        yield_per_acre, price_per_tonne = crops[crop] 
        total_yield = yield_per_acre * free_acres
        chemical_free_realization += total_yield * price_per_tonne
    results["overall_sales"] = total_sales
    results["chemical_free_realization"] = chemical_free_realization
    return results
    

crops = {}
total_acres = int(input("Enter total acre of the farm:"))
acres_segments = int(input("Enter the number of segments the farmer is divided"))
acres_per_segment = int(total_acres / acres_segments)
print("Enter crop details:")
for x in range (acres_segments) :
    crop_name = input("Crop name: ").strip()
    yield_per_acre = float(input(f"Yield per acre for {crop_name} (in tonnes): "))
    price_per_tonne = float(input(f"Price per tonne for {crop_name} (in Rs.): "))
    crops[crop_name] = (yield_per_acre, price_per_tonne)

chemical_free_acres = {}
print("\nEnter chemical-free conversion details (type 'done' to break):")
while True:
    crop_name = input("Crop name for chemical-free conversion: ").strip()
    if crop_name.lower() == "done":
        break
    free_acres = int(input(f"Number of chemical-free acres for {crop_name}: "))
    chemical_free_acres[crop_name] = free_acres

results = sal_and_real(crops, chemical_free_acres, acres_per_segment)

print("\nResults:")
for key, value in results.items():
    print(f"{key}: ₹{value:,.2f}")
