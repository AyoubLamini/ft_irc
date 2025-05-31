#ifndef WEATHERDATA_HPP
#define WEATHERDATA_HPP

#define CITIES_NUMBER 474

struct CityWeather {
    std::string city;
    std::string weather;
};
   
#define CITIES_WEATHER_MAP_INIT { \
    {"Casablanca", "Casablanca, Morocco: 21°C 🌞"}, \
    {"Rabat", "Rabat, Morocco: 19°C ☁️"}, \
    {"Marrakech", "Marrakech, Morocco: 22°C 🌤️"}, \
    {"Tangier", "Tangier, Morocco: 20°C 🌦️"}, \
    {"Agadir", "Agadir, Morocco: 23°C 🌅"}, \
    {"Fes", "Fes, Morocco: 18°C 🌦️"}, \
    {"Chefchaouen", "Chefchaouen, Morocco: 16°C 🌧️"}, \
    {"Ouarzazate", "Ouarzazate, Morocco: 25°C 🌞"}, \
    {"Essaouira", "Essaouira, Morocco: 21°C 🌤️"}, \
    {"Tetouan", "Tetouan, Morocco: 17°C 🌧️"}, \
    {"Sale", "Sale, Morocco: 19°C 🌥️"}, \
    {"El Jadida", "El Jadida, Morocco: 22°C 🌤️"}, \
    {"Nador", "Nador, Morocco: 24°C 🌞"}, \
    {"Safi", "Safi, Morocco: 20°C 🌤️"}, \
    {"Beni Mellal", "Beni Mellal, Morocco: 19°C ☁️"}, \
    {"Taza", "Taza, Morocco: 18°C 🌧️"}, \
    {"Khenifra", "Khenifra, Morocco: 16°C 🌦️"}, \
    {"Midelt", "Midelt, Morocco: 15°C 🌤️"}, \
    {"Dakhla", "Dakhla, Morocco: 27°C 🌞"}, \
    {"Laayoune", "Laayoune, Morocco: 28°C 🌞"}, \
    {"Meknes", "Meknes, Morocco: 20°C 🌥️"}, \
    {"Tiznit", "Tiznit, Morocco: 23°C 🌤️"}, \
    {"Taroudant", "Taroudant, Morocco: 24°C 🌞"}, \
    {"Sidi Ifni", "Sidi Ifni, Morocco: 20°C 🌅"}, \
    {"Errachidia", "Errachidia, Morocco: 25°C 🌞"}, \
    {"Oujda", "Oujda, Morocco: 21°C 🌤️"}, \
    {"Ifrane", "Ifrane, Morocco: 14°C 🌧️"}, \
    {"Azilal", "Azilal, Morocco: 15°C 🌦️"}, \
    {"Ksar el-Kébir", "Ksar el-Kébir, Morocco: 18°C 🌥️"}, \
    {"Al Hoceima", "Al Hoceima, Morocco: 22°C 🌞"}, \
    {"Berkane", "Berkane, Morocco: 23°C 🌤️"}, \
    {"Khouribga", "Khouribga, Morocco: 19°C ☁️"}, \
    {"Tafraoute", "Tafraoute, Morocco: 22°C 🌤️"}, \
    {"Tanger", "Tanger, Morocco: 20°C 🌧️"}, \
    {"Moulay Idriss", "Moulay Idriss, Morocco: 18°C 🌦️"}, \
    {"Rissani", "Rissani, Morocco: 26°C 🌞"}, \
    {"Boujdour", "Boujdour, Morocco: 28°C 🌞"}, \
    {"Azrou", "Azrou, Morocco: 16°C 🌦️"}, \
    {"Algeria", "Algeria, Algeria: 26°C 🌤️"}, \
    {"Tunis", "Tunis, Tunisia: 22°C 🌞"}, \
    {"Cairo", "Cairo, Egypt: 25°C 🌞"}, \
    {"Dubai", "Dubai, UAE: 30°C 🌞"}, \
    {"Abu Dhabi", "Abu Dhabi, UAE: 31°C 🌞"}, \
    {"Doha", "Doha, Qatar: 32°C 🌞"}, \
    {"Muscat", "Muscat, Oman: 30°C 🌤️"}, \
    {"Beirut", "Beirut, Lebanon: 23°C 🌥️"}, \
    {"Amman", "Amman, Jordan: 24°C 🌞"}, \
    {"Baghdad", "Baghdad, Iraq: 28°C 🌞"}, \
    {"Damascus", "Damascus, Syria: 26°C 🌞"}, \
    {"Ankara", "Ankara, Turkey: 22°C 🌤️"}, \
    {"Istanbul", "Istanbul, Turkey: 24°C 🌞"}, \
    {"Athens", "Athens, Greece: 22°C 🌤️"}, \
    {"Rome", "Rome, Italy: 23°C 🌞"}, \
    {"Paris", "Paris, France: 18°C 🌧️"}, \
    {"Berlin", "Berlin, Germany: 16°C 🌧️"}, \
    {"London", "London, UK: 15°C 🌦️"}, \
    {"Madrid", "Madrid, Spain: 24°C 🌞"}, \
    {"Barcelona", "Barcelona, Spain: 25°C 🌤️"}, \
    {"Lisbon", "Lisbon, Portugal: 22°C 🌤️"}, \
    {"Buenos Aires", "Buenos Aires, Argentina: 28°C 🌞"}, \
    {"São Paulo", "São Paulo, Brazil: 26°C 🌤️"}, \
    {"Rio de Janeiro", "Rio de Janeiro, Brazil: 28°C 🌞"}, \
    {"Mexico City", "Mexico City, Mexico: 23°C 🌤️"}, \
    {"Los Angeles", "Los Angeles, USA: 27°C 🌤️"}, \
    {"New York", "New York, USA: 19°C 🌥️"}, \
    {"Toronto", "Toronto, Canada: 17°C 🌧️"}, \
    {"Vancouver", "Vancouver, Canada: 14°C 🌧️"}, \
    {"Sydney", "Sydney, Australia: 25°C 🌞"}, \
    {"Melbourne", "Melbourne, Australia: 22°C 🌤️"}, \
    {"Auckland", "Auckland, New Zealand: 18°C 🌤️"}, \
    {"Wellington", "Wellington, New Zealand: 17°C 🌧️"}, \
    {"Cape Town", "Cape Town, South Africa: 22°C 🌤️"}, \
    {"Durban", "Durban, South Africa: 23°C 🌞"}, \
    {"Lagos", "Lagos, Nigeria: 29°C 🌤️"}, \
    {"Abuja", "Abuja, Nigeria: 27°C 🌞"}, \
    {"Kinshasa", "Kinshasa, DRC: 30°C 🌞"}, \
    {"Nairobi", "Nairobi, Kenya: 25°C 🌤️"}, \
    {"Addis Ababa", "Addis Ababa, Ethiopia: 22°C 🌞"}, \
    {"Accra", "Accra, Ghana: 28°C 🌞"}, \
    {"Lima", "Lima, Peru: 18°C 🌧️"}, \
    {"Santiago", "Santiago, Chile: 23°C 🌞"}, \
    {"Caracas", "Caracas, Venezuela: 28°C 🌞"}, \
    {"Kyoto", "Kyoto, Japan: 20°C 🌤️"}, \
    {"Tokyo", "Tokyo, Japan: 21°C 🌞"}, \
    {"Seoul", "Seoul, South Korea: 19°C 🌥️"}, \
    {"Beijing", "Beijing, China: 24°C 🌞"}, \
    {"Shanghai", "Shanghai, China: 23°C 🌤️"}, \
    {"Hong Kong", "Hong Kong, China: 28°C 🌧️"}, \
    {"Singapore", "Singapore, Singapore: 31°C ⛈️"}, \
    {"Bangkok", "Bangkok, Thailand: 33°C 🌞"}, \
    {"Hanoi", "Hanoi, Vietnam: 29°C 🌤️"}, \
    {"Kuala Lumpur", "Kuala Lumpur, Malaysia: 30°C 🌦️"}, \
    {"Jakarta", "Jakarta, Indonesia: 29°C ⛈️"}, \
    {"Manila", "Manila, Philippines: 32°C 🌞"}, \
    {"New Delhi", "New Delhi, India: 35°C 🌞"}, \
    {"Mumbai", "Mumbai, India: 31°C 🌤️"}, \
    {"Karachi", "Karachi, Pakistan: 34°C 🌞"}, \
    {"Dhaka", "Dhaka, Bangladesh: 30°C 🌧️"}, \
    {"Colombo", "Colombo, Sri Lanka: 29°C 🌤️"}, \
    {"Riyadh", "Riyadh, Saudi Arabia: 36°C 🌞"}, \
    {"Kuwait City", "Kuwait City, Kuwait: 37°C 🌞"}, \
    {"Manama", "Manama, Bahrain: 35°C 🌞"}, \
    {"Sanaa", "Sanaa, Yemen: 26°C 🌤️"}, \
    {"Tehran", "Tehran, Iran: 27°C 🌞"}, \
    {"Baku", "Baku, Azerbaijan: 24°C 🌤️"}, \
    {"Tashkent", "Tashkent, Uzbekistan: 29°C 🌞"}, \
    {"Nur-Sultan", "Nur-Sultan, Kazakhstan: 15°C 🌥️"}, \
    {"Moscow", "Moscow, Russia: 10°C 🌧️"}, \
    {"Saint Petersburg", "Saint Petersburg, Russia: 12°C ☁️"}, \
    {"Kyiv", "Kyiv, Ukraine: 18°C 🌤️"}, \
    {"Warsaw", "Warsaw, Poland: 17°C 🌦️"}, \
    {"Prague", "Prague, Czech Republic: 19°C 🌞"}, \
    {"Budapest", "Budapest, Hungary: 20°C 🌤️"}, \
    {"Vienna", "Vienna, Austria: 21°C 🌞"}, \
    {"Zurich", "Zurich, Switzerland: 17°C 🌦️"}, \
    {"Geneva", "Geneva, Switzerland: 19°C 🌤️"}, \
    {"Brussels", "Brussels, Belgium: 16°C 🌧️"}, \
    {"Amsterdam", "Amsterdam, Netherlands: 15°C 🌦️"}, \
    {"Dublin", "Dublin, Ireland: 14°C 🌧️"}, \
    {"Edinburgh", "Edinburgh, UK: 13°C 🌦️"}, \
    {"Oslo", "Oslo, Norway: 11°C ☁️"}, \
    {"Stockholm", "Stockholm, Sweden: 12°C 🌤️"}, \
    {"Copenhagen", "Copenhagen, Denmark: 13°C 🌦️"}, \
    {"Helsinki", "Helsinki, Finland: 10°C ☁️"}, \
    {"Reykjavik", "Reykjavik, Iceland: 8°C 🌧️"}, \
    {"Montreal", "Montreal, Canada: 16°C 🌦️"}, \
    {"Chicago", "Chicago, USA: 20°C 🌤️"}, \
    {"Boston", "Boston, USA: 18°C 🌧️"}, \
    {"Washington D.C.", "Washington D.C., USA: 22°C 🌞"}, \
    {"Miami", "Miami, USA: 29°C 🌤️"}, \
    {"Dallas", "Dallas, USA: 28°C 🌞"}, \
    {"Houston", "Houston, USA: 29°C 🌤️"}, \
    {"Denver", "Denver, USA: 21°C 🌞"}, \
    {"San Francisco", "San Francisco, USA: 20°C 🌥️"}, \
    {"Seattle", "Seattle, USA: 15°C 🌧️"}, \
    {"Portland", "Portland, USA: 16°C 🌦️"}, \
    {"Las Vegas", "Las Vegas, USA: 30°C 🌞"}, \
    {"Phoenix", "Phoenix, USA: 32°C 🌞"}, \
    {"Honolulu", "Honolulu, USA: 28°C 🌤️"}, \
    {"Anchorage", "Anchorage, USA: 7°C ☁️"}, \
    {"Guadalajara", "Guadalajara, Mexico: 26°C 🌞"}, \
    {"Monterrey", "Monterrey, Mexico: 28°C 🌞"}, \
    {"Havana", "Havana, Cuba: 29°C 🌤️"}, \
    {"Santo Domingo", "Santo Domingo, Dominican Republic: 30°C 🌞"}, \
    {"San Juan", "San Juan, Puerto Rico: 28°C 🌤️"}, \
    {"Kingston", "Kingston, Jamaica: 29°C 🌦️"}, \
    {"Panama City", "Panama City, Panama: 30°C ⛈️"}, \
    {"Bogotá", "Bogotá, Colombia: 19°C 🌧️"}, \
    {"Quito", "Quito, Ecuador: 20°C 🌦️"}, \
    {"La Paz", "La Paz, Bolivia: 15°C 🌦️"}, \
    {"Brasília", "Brasília, Brazil: 27°C 🌤️"}, \
    {"Montevideo", "Montevideo, Uruguay: 25°C 🌞"}, \
    {"Asunción", "Asunción, Paraguay: 29°C 🌞"}, \
    {"Perth", "Perth, Australia: 24°C 🌞"}, \
    {"Brisbane", "Brisbane, Australia: 26°C 🌤️"}, \
    {"Adelaide", "Adelaide, Australia: 23°C 🌞"}, \
    {"Hobart", "Hobart, Australia: 19°C 🌦️"}, \
    {"Christchurch", "Christchurch, New Zealand: 16°C 🌦️"}, \
    {"Suva", "Suva, Fiji: 29°C 🌧️"}, \
    {"Nouméa", "Nouméa, New Caledonia: 27°C 🌤️"}, \
    {"Port Moresby", "Port Moresby, Papua New Guinea: 31°C ⛈️"}, \
    {"Johannesburg", "Johannesburg, South Africa: 20°C 🌤️"}, \
    {"Dar es Salaam", "Dar es Salaam, Tanzania: 30°C 🌞"}, \
    {"Kampala", "Kampala, Uganda: 26°C 🌦️"}, \
    {"Kigali", "Kigali, Rwanda: 24°C 🌧️"}, \
    {"Luanda", "Luanda, Angola: 28°C 🌤️"}, \
    {"Brazzaville", "Brazzaville, Congo: 29°C 🌞"}, \
    {"Yaoundé", "Yaoundé, Cameroon: 27°C 🌧️"}, \
    {"Abidjan", "Abidjan, Ivory Coast: 29°C 🌤️"}, \
    {"Dakar", "Dakar, Senegal: 27°C 🌞"}, \
    {"Bamako", "Bamako, Mali: 34°C 🌞"}, \
    {"Niamey", "Niamey, Niger: 38°C 🌞"}, \
    {"Khartoum", "Khartoum, Sudan: 39°C 🌞"}, \
    {"Djibouti City", "Djibouti City, Djibouti: 35°C 🌞"}, \
    {"Antananarivo", "Antananarivo, Madagascar: 21°C 🌤️"}, \
    {"Port Louis", "Port Louis, Mauritius: 27°C 🌞"}, \
    {"Victoria", "Victoria, Seychelles: 28°C 🌤️"}, \
    {"Algiers", "Algiers, Algeria: 25°C 🌞"}, \
    {"Tripoli", "Tripoli, Libya: 27°C 🌤️"}, \
    {"Juba", "Juba, South Sudan: 32°C 🌞"}, \
    {"Gaborone", "Gaborone, Botswana: 26°C 🌞"}, \
    {"Harare", "Harare, Zimbabwe: 24°C 🌤️"}, \
    {"Lusaka", "Lusaka, Zambia: 25°C 🌞"}, \
    {"Lilongwe", "Lilongwe, Malawi: 23°C 🌤️"}, \
    {"Maputo", "Maputo, Mozambique: 28°C 🌞"}, \
    {"Windhoek", "Windhoek, Namibia: 25°C 🌞"}, \
    {"Gaza", "Gaza, Palestine: 26°C 🌞"}, \
    {"Ramallah", "Ramallah, Palestine: 23°C 🌤️"}, \
    {"Jerusalem", "Jerusalem, Israel: 24°C 🌞"}, \
    {"Tel Aviv", "Tel Aviv, Israel: 25°C 🌤️"}, \
    {"Nicosia", "Nicosia, Cyprus: 26°C 🌞"}, \
    {"Valletta", "Valletta, Malta: 24°C 🌞"}, \
    {"Sarajevo", "Sarajevo, Bosnia and Herzegovina: 20°C 🌤️"}, \
    {"Belgrade", "Belgrade, Serbia: 21°C 🌞"}, \
    {"Zagreb", "Zagreb, Croatia: 22°C 🌤️"}, \
    {"Ljubljana", "Ljubljana, Slovenia: 19°C 🌦️"}, \
    {"Bratislava", "Bratislava, Slovakia: 20°C 🌤️"}, \
    {"Sofia", "Sofia, Bulgaria: 20°C 🌞"}, \
    {"Bucharest", "Bucharest, Romania: 21°C 🌞"}, \
    {"Chisinau", "Chisinau, Moldova: 20°C 🌤️"}, \
    {"Minsk", "Minsk, Belarus: 15°C 🌦️"}, \
    {"Riga", "Riga, Latvia: 14°C ☁️"}, \
    {"Tallinn", "Tallinn, Estonia: 13°C ☁️"}, \
    {"Vilnius", "Vilnius, Lithuania: 15°C 🌤️"}, \
    {"Tirana", "Tirana, Albania: 23°C 🌞"}, \
    {"Podgorica", "Podgorica, Montenegro: 24°C 🌞"}, \
    {"Pristina", "Pristina, Kosovo: 21°C 🌤️"}, \
    {"Skopje", "Skopje, North Macedonia: 22°C 🌞"}, \
    {"Izmir", "Izmir, Turkey: 25°C 🌞"}, \
    {"Antalya", "Antalya, Turkey: 27°C 🌞"}, \
    {"Bursa", "Bursa, Turkey: 20°C 🌤️"}, \
    {"Trabzon", "Trabzon, Turkey: 19°C 🌦️"}, \
    {"Erbil", "Erbil, Iraq: 30°C 🌞"}, \
    {"Basra", "Basra, Iraq: 33°C 🌞"}, \
    {"Kirkuk", "Kirkuk, Iraq: 31°C 🌞"}, \
    {"Aleppo", "Aleppo, Syria: 27°C 🌞"}, \
    {"Latakia", "Latakia, Syria: 26°C 🌤️"}, \
    {"Homs", "Homs, Syria: 25°C 🌞"}, \
    {"Yerevan", "Yerevan, Armenia: 25°C 🌞"}, \
    {"Tbilisi", "Tbilisi, Georgia: 23°C 🌤️"}, \
    {"Ashgabat", "Ashgabat, Turkmenistan: 32°C 🌞"}, \
    {"Dushanbe", "Dushanbe, Tajikistan: 28°C 🌞"}, \
    {"Bishkek", "Bishkek, Kyrgyzstan: 26°C 🌞"}, \
    {"Ulaanbaatar", "Ulaanbaatar, Mongolia: 18°C 🌤️"}, \
    {"Pyongyang", "Pyongyang, North Korea: 20°C 🌤️"}, \
    {"Busan", "Busan, South Korea: 21°C 🌤️"}, \
    {"Osaka", "Osaka, Japan: 22°C 🌞"}, \
    {"Nagoya", "Nagoya, Japan: 21°C 🌤️"}, \
    {"Sapporo", "Sapporo, Japan: 15°C 🌦️"}, \
    {"Hiroshima", "Hiroshima, Japan: 20°C 🌤️"}, \
    {"Fukuoka", "Fukuoka, Japan: 22°C 🌞"}, \
    {"Macau", "Macau, China: 29°C 🌧️"}, \
    {"Taipei", "Taipei, Taiwan: 27°C 🌤️"}, \
    {"Kaohsiung", "Kaohsiung, Taiwan: 28°C 🌞"}, \
    {"Cebu City", "Cebu City, Philippines: 31°C 🌤️"}, \
    {"Davao City", "Davao City, Philippines: 30°C ⛈️"}, \
    {"Bandung", "Bandung, Indonesia: 26°C 🌦️"}, \
    {"Surabaya", "Surabaya, Indonesia: 30°C 🌞"}, \
    {"Ho Chi Minh City", "Ho Chi Minh City, Vietnam: 31°C 🌞"}, \
    {"Phnom Penh", "Phnom Penh, Cambodia: 32°C 🌞"}, \
    {"Vientiane", "Vientiane, Laos: 33°C 🌤️"}, \
    {"Yangon", "Yangon, Myanmar: 34°C 🌞"}, \
    {"Chiang Mai", "Chiang Mai, Thailand: 33°C 🌤️"}, \
    {"Pattaya", "Pattaya, Thailand: 32°C 🌞"}, \
    {"Kochi", "Kochi, India: 30°C 🌧️"}, \
    {"Chennai", "Chennai, India: 34°C 🌞"}, \
    {"Hyderabad", "Hyderabad, India: 33°C 🌤️"}, \
    {"Bengaluru", "Bengaluru, India: 29°C 🌦️"}, \
    {"Lahore", "Lahore, Pakistan: 36°C 🌞"}, \
    {"Islamabad", "Islamabad, Pakistan: 32°C 🌞"}, \
    {"Chittagong", "Chittagong, Bangladesh: 29°C 🌧️"}, \
    {"Kathmandu", "Kathmandu, Nepal: 25°C 🌤️"}, \
    {"Thimphu", "Thimphu, Bhutan: 20°C 🌦️"}, \
    {"Malé", "Malé, Maldives: 29°C 🌤️"}, \
    {"Naypyidaw", "Naypyidaw, Myanmar: 33°C 🌞"}, \
    {"Dammam", "Dammam, Saudi Arabia: 38°C 🌞"}, \
    {"Jeddah", "Jeddah, Saudi Arabia: 34°C 🌞"}, \
    {"Mecca", "Mecca, Saudi Arabia: 39°C 🌞"}, \
    {"Medina", "Medina, Saudi Arabia: 37°C 🌞"}, \
    {"Suez", "Suez, Egypt: 26°C 🌞"}, \
    {"Alexandria", "Alexandria, Egypt: 24°C 🌤️"}, \
    {"Luxor", "Luxor, Egypt: 36°C 🌞"}, \
    {"Aswan", "Aswan, Egypt: 38°C 🌞"}, \
    {"Mersa Matruh", "Mersa Matruh, Egypt: 22°C 🌤️"}, \
    {"Port Said", "Port Said, Egypt: 23°C 🌞"}, \
    {"Benghazi", "Benghazi, Libya: 26°C 🌞"}, \
    {"Misrata", "Misrata, Libya: 27°C 🌤️"}, \
    {"Oran", "Oran, Algeria: 24°C 🌞"}, \
    {"Constantine", "Constantine, Algeria: 23°C 🌤️"}, \
    {"Annaba", "Annaba, Algeria: 22°C 🌦️"}, \
    {"Sfax", "Sfax, Tunisia: 23°C 🌞"}, \
    {"Sousse", "Sousse, Tunisia: 21°C 🌤️"}, \
    {"Monastir", "Monastir, Tunisia: 22°C 🌞"}, \
    {"Ouagadougou", "Ouagadougou, Burkina Faso: 36°C 🌞"}, \
    {"Nouakchott", "Nouakchott, Mauritania: 32°C 🌞"}, \
    {"Conakry", "Conakry, Guinea: 30°C 🌦️"}, \
    {"Freetown", "Freetown, Sierra Leone: 29°C ⛈️"}, \
    {"Monrovia", "Monrovia, Liberia: 28°C 🌧️"}, \
    {"Lomé", "Lomé, Togo: 29°C 🌤️"}, \
    {"Cotonou", "Cotonou, Benin: 28°C 🌦️"}, \
    {"Port Harcourt", "Port Harcourt, Nigeria: 28°C 🌤️"}, \
    {"Douala", "Douala, Cameroon: 28°C ⛈️"}, \
    {"Malabo", "Malabo, Equatorial Guinea: 29°C 🌦️"}, \
    {"Libreville", "Libreville, Gabon: 28°C 🌧️"}, \
    {"Pretoria", "Pretoria, South Africa: 23°C 🌞"}, \
    {"Bloemfontein", "Bloemfontein, South Africa: 21°C 🌤️"}, \
    {"Port Elizabeth", "Port Elizabeth, South Africa: 20°C 🌦️"}, \
    {"Moroni", "Moroni, Comoros: 29°C 🌤️"}, \
    {"Asmara", "Asmara, Eritrea: 26°C 🌤️"}, \
    {"Zanzibar City", "Zanzibar City, Tanzania: 29°C 🌤️"}, \
    {"Bujumbura", "Bujumbura, Burundi: 25°C 🌤️"}, \
    {"Dodoma", "Dodoma, Tanzania: 27°C 🌞"}, \
    {"Mombasa", "Mombasa, Kenya: 30°C 🌞"}, \
    {"Blantyre", "Blantyre, Malawi: 22°C 🌞"}, \
    {"Tripoli, Lebanon", "Tripoli, Lebanon: 24°C 🌤️"}, \
    {"Palmyra", "Palmyra, Syria: 28°C 🌞"}, \
    {"Diyarbakır", "Diyarbakır, Turkey: 27°C 🌞"}, \
    {"Gaziantep", "Gaziantep, Turkey: 26°C 🌞"}, \
    {"Adana", "Adana, Turkey: 29°C 🌞"}, \
    {"Konya", "Konya, Turkey: 21°C 🌤️"}, \
    {"Sivas", "Sivas, Turkey: 19°C 🌦️"}, \
    {"Erzurum", "Erzurum, Turkey: 15°C 🌤️"}, \
    {"Van", "Van, Turkey: 17°C 🌞"}, \
    {"Nakhchivan", "Nakhchivan, Azerbaijan: 28°C 🌞"}, \
    {"Ganja", "Ganja, Azerbaijan: 26°C 🌞"}, \
    {"Tabriz", "Tabriz, Iran: 25°C 🌞"}, \
    {"Mashhad", "Mashhad, Iran: 29°C 🌞"}, \
    {"Isfahan", "Isfahan, Iran: 28°C 🌞"}, \
    {"Shiraz", "Shiraz, Iran: 27°C 🌞"}, \
    {"Bandar Abbas", "Bandar Abbas, Iran: 34°C 🌞"}, \
    {"Kabul", "Kabul, Afghanistan: 24°C 🌞"}, \
    {"Herat", "Herat, Afghanistan: 27°C 🌞"}, \
    {"Kandahar", "Kandahar, Afghanistan: 30°C 🌞"}, \
    {"Peshawar", "Peshawar, Pakistan: 33°C 🌞"}, \
    {"Quetta", "Quetta, Pakistan: 29°C 🌞"}, \
    {"Multan", "Multan, Pakistan: 35°C 🌞"}, \
    {"Srinagar", "Srinagar, India: 23°C 🌤️"}, \
    {"Amritsar", "Amritsar, India: 35°C 🌞"}, \
    {"Jaipur", "Jaipur, India: 36°C 🌞"}, \
    {"Ahmedabad", "Ahmedabad, India: 37°C 🌞"}, \
    {"Pune", "Pune, India: 30°C 🌤️"}, \
    {"Surat", "Surat, India: 36°C 🌞"}, \
    {"Bhopal", "Bhopal, India: 34°C 🌞"}, \
    {"Lucknow", "Lucknow, India: 35°C 🌞"}, \
    {"Patna", "Patna, India: 32°C 🌞"}, \
    {"Guwahati", "Guwahati, India: 30°C 🌦️"}, \
    {"Visakhapatnam", "Visakhapatnam, India: 32°C 🌞"}, \
    {"Madurai", "Madurai, India: 35°C 🌞"}, \
    {"Jaffna", "Jaffna, Sri Lanka: 30°C 🌤️"}, \
    {"Kandy", "Kandy, Sri Lanka: 27°C 🌦️"}, \
    {"Cox's Bazar", "Cox's Bazar, Bangladesh: 28°C 🌤️"}, \
    {"Pokhara", "Pokhara, Nepal: 26°C 🌤️"}, \
    {"Paro", "Paro, Bhutan: 19°C 🌤️"}, \
    {"Mandalay", "Mandalay, Myanmar: 34°C 🌞"}, \
    {"Luang Prabang", "Luang Prabang, Laos: 32°C 🌤️"}, \
    {"Siem Reap", "Siem Reap: 31°C 🌞"}, \
    {"Da Nang", "Da Nang, Vietnam: 30°C 🌞"}, \
    {"Hue", "Hue, Vietnam: 29°C 🌤️"}, \
    {"Penang", "Penang, Malaysia: 31°C 🌤️"}, \
    {"Johor Bahru", "Johor Bahru, Malaysia: 30°C 🌞"}, \
    {"Ipoh", "Ipoh, Malaysia: 29°C 🌦️"}, \
    {"Kota Kinabalu", "Kota Kinabalu, Malaysia: 30°C 🌤️"}, \
    {"Kuching", "Kuching, Malaysia: 28°C 🌧️"}, \
    {"Bandar Seri Begawan", "Bandar Seri Begawan, Brunei: 30°C 🌦️"}, \
    {"Medan", "Medan, Indonesia: 29°C 🌧️"}, \
    {"Makassar", "Makassar, Indonesia: 28°C 🌤️"}, \
    {"Denpasar", "Denpasar, Indonesia: 29°C 🌞"}, \
    {"Baguio", "Baguio, Philippines: 24°C 🌦️"}, \
    {"Incheon", "Incheon, South Korea: 19°C 🌥️"}, \
    {"Daegu", "Daegu, South Korea: 22°C 🌞"}, \
    {"Gwangju", "Gwangju, South Korea: 21°C 🌤️"}, \
    {"Sendai", "Sendai, Japan: 18°C 🌦️"}, \
    {"Darwin", "Darwin, Australia: 31°C 🌞"}, \
    {"Canberra", "Canberra, Australia: 20°C 🌤️"}, \
    {"Gold Coast", "Gold Coast, Australia: 27°C 🌞"}, \
    {"Cairns", "Cairns, Australia: 29°C 🌦️"}, \
    {"Dunedin", "Dunedin, New Zealand: 15°C ☁️"}, \
    {"Port Vila", "Port Vila, Vanuatu: 28°C ⛈️"}, \
    {"Apia", "Apia, Samoa: 29°C 🌦️"}, \
    {"Nukuʻalofa", "Nukuʻalofa, Tonga: 27°C 🌤️"}, \
    {"Majuro", "Majuro, Marshall Islands: 28°C 🌤️"}, \
    {"Palikir", "Palikir, Micronesia: 29°C ⛈️"}, \
    {"Funafuti", "Funafuti, Tuvalu: 29°C 🌦️"}, \
    {"Tarawa", "Tarawa, Kiribati: 30°C 🌞"}, \
    {"Yaren", "Yaren, Nauru: 31°C 🌞"}, \
    {"Honiara", "Honiara, Solomon Islands: 30°C ⛈️"}, \
    {"Papeete", "Papeete, French Polynesia: 28°C 🌤️"}, \
    {"Valparaíso", "Valparaíso, Chile: 20°C 🌤️"}, \
    {"Concepción", "Concepción, Chile: 19°C 🌦️"}, \
    {"Santa Cruz de la Sierra", "Santa Cruz de la Sierra, Bolivia: 28°C 🌞"}, \
    {"Sucre", "Sucre, Bolivia: 23°C 🌤️"}, \
    {"Córdoba", "Córdoba, Argentina: 26°C 🌞"}, \
    {"Rosario", "Rosario, Argentina: 27°C 🌞"}, \
    {"Mendoza", "Mendoza, Argentina: 25°C 🌞"}, \
    {"Salvador", "Salvador, Brazil: 29°C 🌞"}, \
    {"Fortaleza", "Fortaleza, Brazil: 30°C 🌞"}, \
    {"Recife", "Recife, Brazil: 29°C 🌤️"}, \
    {"Manaus", "Manaus, Brazil: 30°C ⛈️"}, \
    {"Belo Horizonte", "Belo Horizonte, Brazil: 26°C 🌞"}, \
    {"Curitiba", "Curitiba, Brazil: 24°C 🌤️"}, \
    {"Porto Alegre", "Porto Alegre, Brazil: 25°C 🌞"}, \
    {"Arequipa", "Arequipa, Peru: 21°C 🌤️"}, \
    {"Cusco", "Cusco, Peru: 17°C 🌦️"}, \
    {"Guayaquil", "Guayaquil, Ecuador: 30°C 🌞"}, \
    {"Medellín", "Medellín, Colombia: 25°C 🌦️"}, \
    {"Cali", "Cali, Colombia: 28°C 🌤️"}, \
    {"Barranquilla", "Barranquilla, Colombia: 31°C 🌞"}, \
    {"Maracaibo", "Maracaibo, Venezuela: 30°C 🌞"}, \
    {"Valencia", "Valencia, Venezuela: 29°C 🌤️"}, \
    {"Georgetown", "Georgetown, Guyana: 29°C 🌧️"}, \
    {"Paramaribo", "Paramaribo, Suriname: 28°C 🌦️"}, \
    {"Cayenne", "Cayenne, French Guiana: 27°C 🌧️"}, \
    {"San José", "San José, Costa Rica: 25°C 🌤️"}, \
    {"Managua", "Managua, Nicaragua: 32°C 🌞"}, \
    {"Tegucigalpa", "Tegucigalpa, Honduras: 30°C 🌞"}, \
    {"San Salvador", "San Salvador, El Salvador: 29°C 🌞"}, \
    {"Guatemala City", "Guatemala City, Guatemala: 28°C 🌤️"}, \
    {"Belmopan", "Belmopan, Belize: 29°C 🌞"}, \
    {"Port-au-Prince", "Port-au-Prince, Haiti: 30°C 🌤️"}, \
    {"Nassau", "Nassau, Bahamas: 28°C 🌞"}, \
    {"Bridgetown", "Bridgetown, Barbados: 29°C 🌤️"}, \
    {"Port of Spain", "Port of Spain, Trinidad and Tobago: 30°C 🌞"}, \
    {"Castries", "Castries, Saint Lucia: 29°C 🌤️"}, \
    {"St. John's", "St. John's, Antigua and Barbuda: 28°C 🌞"}, \
    {"Roseau", "Roseau, Dominica: 28°C 🌧️"}, \
    {"Basseterre", "Basseterre, Saint Kitts and Nevis: 29°C 🌤️"}, \
    {"St. George's", "St. George's, Grenada: 29°C 🌤️"}, \
    {"Kingstown", "Kingstown, Saint Vincent and the Grenadines: 28°C 🌤️"}, \
    {"Tijuana", "Tijuana, Mexico: 22°C 🌤️"}, \
    {"Cancún", "Cancún, Mexico: 30°C 🌞"}, \
    {"New Orleans", "New Orleans, USA: 27°C 🌤️"}, \
    {"Atlanta", "Atlanta, USA: 26°C 🌞"}, \
    {"Orlando", "Orlando, USA: 28°C 🌞"}, \
    {"Philadelphia", "Philadelphia, USA: 20°C 🌤️"}, \
    {"Detroit", "Detroit, USA: 19°C 🌦️"}, \
    {"Minneapolis", "Minneapolis, USA: 17°C 🌤️"}, \
    {"St. Louis", "St. Louis, USA: 22°C 🌞"}, \
    {"Kansas City", "Kansas City, USA: 23°C 🌞"}, \
    {"Salt Lake City", "Salt Lake City, USA: 20°C 🌞"}, \
    {"Boise", "Boise, USA: 18°C 🌤️"}, \
    {"Calgary", "Calgary, Canada: 15°C 🌤️"}, \
    {"Edmonton", "Edmonton, Canada: 14°C 🌦️"}, \
    {"Winnipeg", "Winnipeg, Canada: 12°C 🌤️"}, \
    {"Ottawa", "Ottawa, Canada: 17°C 🌤️"}, \
    {"Quebec City", "Quebec City, Canada: 15°C 🌦️"}, \
    {"Halifax", "Halifax, Canada: 13°C 🌧️"}, \
    {"St. John's, Canada", "St. John's, Canada: 10°C 🌦️"}, \
    {"Nuuk", "Nuuk, Greenland: 2°C 🌨️"}, \
    {"Kraków", "Kraków, Poland: 18°C 🌤️"}, \
    {"Strasbourg", "Strasbourg, France: 19°C 🌤️"}, \
    {"Lyon", "Lyon, France: 20°C 🌞"}, \
    {"Marseille", "Marseille, France: 22°C 🌞"}, \
    {"Nice", "Nice, France: 23°C 🌞"}, \
    {"Bordeaux", "Bordeaux, France: 20°C 🌤️"}, \
    {"Nantes", "Nantes, France: 18°C 🌦️"}, \
    {"Lille", "Lille, France: 17°C 🌧️"}, \
    {"Bern", "Bern, Switzerland: 18°C 🌤️"}, \
    {"Milan", "Milan, Italy: 21°C 🌞"}, \
    {"Venice", "Venice, Italy: 22°C 🌞"}, \
    {"Florence", "Florence, Italy: 23°C 🌞"}, \
    {"Naples", "Naples, Italy: 24°C 🌞"}, \
    {"Palermo", "Palermo, Italy: 25°C 🌞"}, \
    {"Bologna", "Bologna, Italy: 22°C 🌤️"}, \
    {"Turin", "Turin, Italy: 20°C 🌤️"}, \
    {"Vatican City", "Vatican City, Vatican City: 23°C 🌞"}, \
    {"San Marino", "San Marino, San Marino: 22°C 🌞"}, \
    {"Monaco", "Monaco, Monaco: 24°C 🌞"}, \
    {"Porto", "Porto, Portugal: 21°C 🌤️"}, \
    {"Seville", "Seville, Spain: 28°C 🌞"}, \
    {"Valencia", "Valencia, Spain: 26°C 🌞"}, \
    {"Malaga", "Malaga, Spain: 27°C 🌞"}, \
    {"Palma de Mallorca", "Palma de Mallorca, Spain: 25°C 🌞"}, \
    {"Santander", "Santander, Spain: 19°C 🌦️"}, \
    {"Bilbao", "Bilbao, Spain: 20°C 🌤️"}, \
    {"Munich", "Munich, Germany: 18°C 🌤️"}, \
    {"Hamburg", "Hamburg, Germany: 15°C 🌦️"}, \
    {"Frankfurt", "Frankfurt, Germany: 17°C 🌦️"}, \
    {"Cologne", "Cologne, Germany: 16°C 🌧️"}, \
    {"Dresden", "Dresden, Germany: 17°C 🌤️"}, \
    {"Leipzig", "Leipzig, Germany: 16°C 🌦️"}, \
    {"Stuttgart", "Stuttgart, Germany: 18°C 🌤️"}, \
    {"Hannover", "Hannover, Germany: 15°C 🌦️"}, \
    {"Bremen", "Bremen, Germany: 14°C 🌧️"}, \
    {"Cannes", "Cannes, France: 23°C 🌞"}, \
    {"Ajaccio", "Ajaccio, France: 21°C 🌤️"}, \
    {"Heraklion", "Heraklion, Greece: 24°C 🌞"}, \
    {"Thessaloniki", "Thessaloniki, Greece: 23°C 🌞"}, \
    {"Mykonos", "Mykonos, Greece: 22°C 🌞"}, \
    {"Santorini", "Santorini, Greece: 23°C 🌞"}, \
    {"Rhodes", "Rhodes, Greece: 25°C 🌞"}, \
    {"Corfu", "Corfu, Greece: 23°C 🌤️"}, \
    {"Dubrovnik", "Dubrovnik, Croatia: 23°C 🌞"}, \
    {"Split", "Split, Croatia: 24°C 🌞"}, \
    {"Pula", "Pula, Croatia: 21°C 🌤️"} \
}

#endif 