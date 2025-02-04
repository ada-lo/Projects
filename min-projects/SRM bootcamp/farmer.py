total_land = 80
no_of_segments = 5
each_segment = total_land // no_of_segments

tamoto_yield_tonnes = (each_segment * 0.3 * 10) + (each_segment * 0.7 *12)
tomato_sales_amount = int(tamoto_yield_tonnes * 1000 * 7)

potato_yeild_tonnes = each_segment * 10
potato_sales_amount = potato_yeild_tonnes * 1000 * 20

cabbage_yeild_tonnes = each_segment * 14
cabbage_sales_amount = cabbage_yeild_tonnes * 1000 * 24

sunflower_yeild_tonnes = each_segment * 0.7
sunflower_sales_amount = int(sunflower_yeild_tonnes * 1000 * 200)

sugarcane_yeild_tonnes = each_segment * 45
sugarcane_sales_amount = sugarcane_yeild_tonnes * 4000

total_sales_amount = tomato_sales_amount + potato_sales_amount + cabbage_sales_amount + sunflower_sales_amount + sugarcane_sales_amount

print(f'Sales from Tomato     is {tomato_sales_amount}')
print(f'Sales from Potatoes   is {potato_sales_amount}')
print(f'Sales from Cabbage    is {cabbage_sales_amount}')
print(f'Sales from Sunflower  is {sunflower_sales_amount}')
print(f'Sales from Sugarcane  is {sugarcane_sales_amount}')

print(f'Total Sales Amount is {total_sales_amount}')

chemical_free_total_sales = tomato_sales_amount + potato_sales_amount + cabbage_sales_amount + sunflower_sales_amount

print(f'Total Sales Amount from Chemical Free Farming is {chemical_free_total_sales}')
