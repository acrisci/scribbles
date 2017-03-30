#!/usr/bin/env python
from flask import Flask, request, make_response
from testapp.database import db_session, init_db
from testapp.models import User
import json

init_db()

app = Flask(__name__)

@app.route('/', methods=['GET'])
def hello_world():
    return 'Hello, World!'

@app.route('/users', methods=['GET'])
def get_users():
    users = User.query.all()
    return json.dumps([u.as_dict() for u in users], indent=2)

@app.route('/users/<user_id>', methods=['GET'])
def get_user(user_id):
    user = User.query.get(user_id)
    return json.dumps(user.as_dict(), indent=2)

@app.route('/users', methods=['POST'])
def create_user():
    data = request.get_json(force=True)
    user = User(data['name'], data['email'])
    db_session.add(user)
    db_session.commit()
    return (json.dumps(user.as_dict(), indent=2), 201)

@app.teardown_appcontext
def shutdown_session(exception=None):
    db_session.remove()
