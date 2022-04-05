import os
from pymongo import MongoClient
from flask import Flask
from flask import render_template
from flask import request
from flask import redirect
from datetime import datetime
import pytz
app = Flask(__name__)

cluster = MongoClient("") #mongodb atlas connect id
db = cluster['overspeed']


@app.route("/")
def hello():
    return render_template('index.html')


@app.route("/secret/") #this is hidden so that no one enters random values in the database
def add_value():
    try:
        lat = request.args.get("lat")
        lon = request.args.get("lon")
        car = request.args.get("car_no")
        station = request.args.get('station')
        print(f'latitude: {lat}, longitude: {lon}')
        x = datetime.now(pytz.timezone("Asia/Calcutta"))
        date = x.strftime("%d-%m-%Y")
        time = x.strftime("%I:%M:%S %p")
        arra = {"Date": date, "Time": time, "latitude": lat,
                "longitude": lon, "Car Number": car}
        if car is None or lon is None or lat is None or station is None:
            return "Aw Snap! Error Occured! Beep Boop Boop Beep!"
        table = db['station_one'] if int(station) == 1 else db['station_two']
        table.insert_one(arra)
        return "values added"
    except:
        return "Aw Snap! Error Occured! Beep Boop Boop Beep!"


@app.route('/admin')
def login():
    return render_template('login.html')


@app.route('/admin', methods=['POST'])
def my_form_post():
    mail = request.form['text']
    password = request.form['password']
    table = db['users']
    # print(processed_mail)
    # print(processed_password)
    a = table.find_one(
        {'mail': mail, 'password': password})
    if a is None:
        return "Aw Snap! Wrong Password or Email!"
    sta1 = db['station_one']
    b = sta1.find({})
    sta2 = db['station_two']
    c = sta2.find({})
    return render_template('admin.html', cases1=b, cases2=c)

@app.route('/station_1')
def sta1():
    sta = db['station_one']
    b = sta.find({})
    return render_template('station one.html', cases=b)

@app.route('/station_2')
def sta2():
    while True:
        sta = db['station_two']
        b = sta.find({})
        return render_template('station two.html', cases=b)

@app.route('/delete_1/')
def delete():
    date = request.args.get("date")
    time = request.args.get("time")
    car = request.args.get("car")
    todel = {"Date": date,
             "Time": time,
             'Car Number': car}
    print(todel)
    sta = db['station_one']
    sta.delete_one(todel)
    return redirect('/station_1')

@app.route('/contact')
def form():
    return render_template('contact.html')

@app.route('/contact' , methods=['POST'])
def formsubmit():
    name = request.form['name']
    email = request.form['email']
    querry = request.form['Querry']
    array = {"name":name,"email":email,"querry":querry}
    table = db['Querries']
    table.insert_one(array)
    return render_template('sucsess.html')

@app.route('/delete_2/')
def delete2():
    date = request.args.get("date")
    time = request.args.get("time")
    car = request.args.get("car")
    todel = {"Date": date,
             "Time": time,
             'Car Number': car}
    print(todel)
    sta = db['station_two']
    sta.delete_one(todel)
    return redirect('/station_2')

app.run(host="localhost", port=8080)
