function [journey, new_case] = revise(retrieved_cases, new_case, new_price)
    
    retrieved_codes = retrieved_cases{:,1};
    code = str2double('-');
        
    while isnan(code) || fix(code) ~= code || ismember(code, retrieved_codes) == 0
        fprintf('From the retrieved cases, which is the one that better matches your journey?\n');
        code = str2double(input('Journey Code: ','s'));
    end
    
    journey = fix(code);
    

    %REVISE HOLIDAY_TYPE
    fprintf('\nQuer alterar o tipo de férias? (y/n)\n');
    option = input('Option: ', 's');
    if option == 'y' || option == 'Y'
        lista ={'Active', 'Bathing', 'City', 'Education', 'Language', 'Recreation', 'Skiing', 'Wandering'};
        valor = input('Introduza novo val.:','s');
        while ~ismember(lista,valor)
            valor = input('Introduza novo val.:','s')
        end
        new_case.holiday_type = valor;
    end
    



    fprintf('\nUpdate your journey price with the new estimated value? (y/n)\n');
    option = input('Option: ', 's');
    if option == 'y' || option == 'Y'
        new_case.price = new_price;
    end

    %REVISE NUMBER_PERSONS
    fprintf('\nQuer alterar o num. de pessoas? (y/n)\n');
    option = input('Option: ', 's');
    if option == 'y' || option == 'Y'
        valor = str2double('-');
        
        while isnan(valor) 
                valor = str2double(input('Introduza novo val.:','s'));
                new_case.number_persons = valor;
        end
    end

end

