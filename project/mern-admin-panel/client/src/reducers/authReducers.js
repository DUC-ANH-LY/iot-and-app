import {
    SET_CURRENT_USER,
    USER_ADD,
    USER_LOADING,
    USER_UPDATE
} from "../actions/types";
// const isEmpty = require("is-empty");
const initialState = {
    isAuthenticated: false,
    user: {},
    loading: false,
};
export default function(state = initialState, action) {
    switch (action.type) {
        case USER_ADD:
            return {
                isAuthenticated: (action.payload != undefined),
                user: action.payload
            };
        case USER_UPDATE:
            return {
                isAuthenticated: (action.payload != undefined),
                user: action.payload,
            };
        case SET_CURRENT_USER:
            return {
                ...state,
                isAuthenticated: (action.payload != undefined),
                user: action.payload
            };
        case USER_LOADING:
            return {
                ...state,
                loading: true
            };
        default:
            return state;
    }
}
